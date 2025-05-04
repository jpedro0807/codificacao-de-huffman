#include <stdio.h>
#include <stdlib.h>
#include "tabela_de_frequencias.h"
#include "huffman.h"
#include "gera_codigos.h"
#include "codigo.h"
#include "myTypesClion.h"

#define NOME_ARQUIVO_ENTRADA "apostilaC.pdf"
#define NOME_ARQUIVO_SAIDA "saida.huff"

int main(void) {
    // Passo 1: Abrir o arquivo original
    FILE* f_in = fopen(NOME_ARQUIVO_ENTRADA, "rb");
    if (f_in == NULL) {
        printf("Erro ao abrir arquivo de entrada!\n");
        return 1;
    }

    // Passo 2: Construir tabela de frequências
    Tabela_de_frequencias tabela;
    nova_tabela_de_frequencias(&tabela);

    U8 byte;
    U64 tamanho_arquivo = 0;
    while (fread(&byte, 1, 1, f_in) == 1) {
        inclua_byte(byte, &tabela);
        tamanho_arquivo++;
    }

    rewind(f_in); // voltar ao início para reler depois

    // Passo 3: Construir árvore de Huffman
    Ptr_de_no_de_arvore_binaria raiz = cria_arvore_huffman(&tabela);

    // Passo 4: Gerar códigos para cada byte
    Codigo codigos[256];
    for (int i = 0; i < 256; i++) {
        novo_codigo(&codigos[i]);
    }

    Codigo codigo_atual;
    novo_codigo(&codigo_atual);
    gera_codigos(raiz, codigos, &codigo_atual);
    free_codigo(&codigo_atual);

    // Passo 5: Abrir arquivo de saída
    FILE* f_out = fopen(NOME_ARQUIVO_SAIDA, "wb");
    if (f_out == NULL) {
        printf("Erro ao criar arquivo de saída!\n");
        fclose(f_in);
        return 1;
    }

    // Passo 6: Escrever tabela de frequências no arquivo
    for (int i = 0; i < 256; i++) {
        U64 freq = (tabela.vetor[i] != NULL) ? tabela.vetor[i]->informacao.frequencia : 0;
        fwrite(&freq, sizeof(U64), 1, f_out);
    }

    // Passo 7: Reservar espaço para quantidade de bits (iremos voltar e escrever depois)
    U64 quantidade_bits = 0;
    long pos_qtd_bits = ftell(f_out);
    fwrite(&quantidade_bits, sizeof(U64), 1, f_out);

    // Passo 8: Codificar e escrever os dados
    U8 buffer = 0;
    int buffer_tamanho = 0;

    while (fread(&byte, 1, 1, f_in) == 1) {
        Codigo* codigo = &codigos[byte];
        for (int bit = 0; bit < codigo->tamanho; bit++) {
            U8 valor = (codigo->byte[bit / 8] >> (7 - (bit % 8))) & 1;
            buffer = (buffer << 1) | valor;
            buffer_tamanho++;

            quantidade_bits++;

            if (buffer_tamanho == 8) {
                fwrite(&buffer, 1, 1, f_out);
                buffer = 0;
                buffer_tamanho = 0;
            }
        }
    }

    // Escrever resto do buffer se sobrou algo
    if (buffer_tamanho > 0) {
        buffer <<= (8 - buffer_tamanho);
        fwrite(&buffer, 1, 1, f_out);
    }

    // Passo 9: Voltar para escrever quantidade de bits no lugar reservado
    fseek(f_out, pos_qtd_bits, SEEK_SET);
    fwrite(&quantidade_bits, sizeof(U64), 1, f_out);

    // Fechar arquivos
    fclose(f_in);
    fclose(f_out);

    // Liberar os códigos
    for (int i = 0; i < 256; i++) {
        free_codigo(&codigos[i]);
    }

    printf("Compressão concluída. Arquivo salvo como '%s'.\n", NOME_ARQUIVO_SAIDA);
    return 0;
}
