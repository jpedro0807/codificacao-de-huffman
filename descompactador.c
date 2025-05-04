#include <stdio.h>
#include <stdlib.h>
#include "tabela_de_frequencias.h"
#include "huffman.h"
#include "myTypesClion.h"

#define NOME_ARQUIVO_ENTRADA "saida.huff"
#define NOME_ARQUIVO_SAIDA "saida.pdf"

int main(void) {
    FILE* f_in = fopen(NOME_ARQUIVO_ENTRADA, "rb");
    if (!f_in) {
        printf("Erro ao abrir o arquivo compactado!\n");
        return 1;
    }

    // Passo 1: Ler a tabela de frequências
    Tabela_de_frequencias tabela;
    nova_tabela_de_frequencias(&tabela);

    for (int i = 0; i < 256; i++) {
        U64 freq;
        fread(&freq, sizeof(U64), 1, f_in);

        if (freq > 0) {
            tabela.vetor[i] = (Ptr_de_no_de_arvore_binaria)malloc(sizeof(Struct_do_no_de_arvore_binaria));
            tabela.vetor[i]->informacao.byte = (U8)i;
            tabela.vetor[i]->informacao.frequencia = freq;
            tabela.vetor[i]->esquerda = NULL;
            tabela.vetor[i]->direita = NULL;
        }
    }

    // Passo 2: Reconstruir a árvore de Huffman
    Ptr_de_no_de_arvore_binaria raiz = cria_arvore_huffman(&tabela);

    // Passo 3: Ler a quantidade de bits
    U64 qtd_bits = 0;
    fread(&qtd_bits, sizeof(U64), 1, f_in);

    // Passo 4: Decodificar os bits e escrever no arquivo final
    FILE* f_out = fopen(NOME_ARQUIVO_SAIDA, "wb");
    if (!f_out) {
        printf("Erro ao criar arquivo de saída!\n");
        return 1;
    }

    Ptr_de_no_de_arvore_binaria atual = raiz;
    int bits_lidos = 0;
    U8 byte;

    while (bits_lidos < qtd_bits) {
        fread(&byte, 1, 1, f_in);
        for (int i = 7; i >= 0 && bits_lidos < qtd_bits; i--) {
            U8 bit = (byte >> i) & 1;
            atual = (bit == 0) ? atual->esquerda : atual->direita;

            if (atual->esquerda == NULL && atual->direita == NULL) {
                fwrite(&atual->informacao.byte, 1, 1, f_out);
                atual = raiz;
            }
            bits_lidos++;
        }
    }

    fclose(f_in);
    fclose(f_out);

    printf("Descompactação concluída. Arquivo salvo como '%s'.\n", NOME_ARQUIVO_SAIDA);
    return 0;
}
