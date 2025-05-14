#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <sys/stat.h>
#include <errno.h>
#include "tabela_de_frequencias.h"
#include "huffman.h"
#include "myTypesClion.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <arquivo_entrada.huff> <arquivo_saida.ext>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Garante que a pasta "saida" exista
    if (mkdir("saida", 0755) != 0 && errno != EEXIST) {
        perror("mkdir(\"saida\")");
        return EXIT_FAILURE;
    }

    // Monta caminhos completos
    char in_path[PATH_MAX], out_path[PATH_MAX];
    // Agora lê da pasta "saida/"
    snprintf(in_path,  sizeof(in_path),  "saida/%s", argv[1]);
    snprintf(out_path, sizeof(out_path), "saida/%s", argv[2]);

    // Abre o .huff para leitura (corrigido para usar in_path)
    FILE *f_in = fopen(in_path, "rb");
    if (!f_in) {
        fprintf(stderr, "Erro ao abrir '%s': %s\n", in_path, strerror(errno));
        return EXIT_FAILURE;
    }

    // Passo 1: ler tabela de frequências
    Tabela_de_frequencias tabela;
    nova_tabela_de_frequencias(&tabela);
    for (int i = 0; i < 256; i++) {
        U64 freq;
        if (fread(&freq, sizeof(U64), 1, f_in) != 1) {
            fprintf(stderr, "Formato inválido (tabela de frequências)\n");
            fclose(f_in);
            return EXIT_FAILURE;
        }
        if (freq > 0) {
            tabela.vetor[i] = malloc(sizeof(Struct_do_no_de_arvore_binaria));
            tabela.vetor[i]->informacao.byte = (U8)i;
            tabela.vetor[i]->informacao.frequencia = freq;
            tabela.vetor[i]->esquerda = NULL;
            tabela.vetor[i]->direita  = NULL;
        }
    }

    // Passo 2: reconstruir árvore de Huffman
    Ptr_de_no_de_arvore_binaria raiz = cria_arvore_huffman(&tabela);

    // Passo 3: ler quantidade total de bits
    U64 qtd_bits;
    if (fread(&qtd_bits, sizeof(U64), 1, f_in) != 1) {
        fprintf(stderr, "Formato inválido (contagem de bits)\n");
        fclose(f_in);
        return EXIT_FAILURE;
    }

    // Abre arquivo de saída
    FILE *f_out = fopen(out_path, "wb");
    if (!f_out) {
        fprintf(stderr, "Erro ao criar '%s': %s\n", out_path, strerror(errno));
        fclose(f_in);
        return EXIT_FAILURE;
    }

    // Passo 4: decodificar bit a bit
    Ptr_de_no_de_arvore_binaria atual = raiz;
    U64 bits_lidos = 0;
    U8 byte;
    while (bits_lidos < qtd_bits && fread(&byte, 1, 1, f_in) == 1) {
        for (int i = 7; i >= 0 && bits_lidos < qtd_bits; i--, bits_lidos++) {
            U8 bit = (byte >> i) & 1;
            atual = (bit == 0) ? atual->esquerda : atual->direita;
            if (!atual->esquerda && !atual->direita) {
                fwrite(&atual->informacao.byte, 1, 1, f_out);
                atual = raiz;
            }
        }
    }

    // Fecha arquivos
    fclose(f_in);
    fclose(f_out);

    printf("Descompactação concluída: %s → %s\n", in_path, out_path);
    return EXIT_SUCCESS;
}
