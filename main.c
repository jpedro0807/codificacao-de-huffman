#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <sys/stat.h>
#include <errno.h>
#include "tabela_de_frequencias.h"
#include "huffman.h"
#include "gera_codigos.h"
#include "codigo.h"
#include "myTypesClion.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <arquivo_entrada.ext>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Garante que a pasta "saida" exista
    if (mkdir("saida", 0755) != 0 && errno != EEXIST) {
        perror("mkdir(\"saida\")");
        return EXIT_FAILURE;
    }

    // Monta caminhos completos
    char in_path[PATH_MAX];
    char out_path[PATH_MAX];
    snprintf(in_path,  sizeof(in_path),  "entrada/%s", argv[1]);

    // Extrai o nome-base (sem extensão) para gerar .huff
    char base[PATH_MAX];
    strncpy(base, argv[1], sizeof(base));
    char *dot = strrchr(base, '.');
    if (dot) *dot = '\0';
    snprintf(out_path, sizeof(out_path), "saida/%s.huff", base);

    // Abre arquivos em binário
    FILE *f_in  = fopen(in_path,  "rb");
    if (!f_in) {
        fprintf(stderr, "Erro ao abrir '%s': %s\n", in_path, strerror(errno));
        return EXIT_FAILURE;
    }
    FILE *f_out = fopen(out_path, "wb");
    if (!f_out) {
        fprintf(stderr, "Erro ao criar '%s': %s\n", out_path, strerror(errno));
        fclose(f_in);
        return EXIT_FAILURE;
    }

    // Passo 1: montar tabela de frequências
    Tabela_de_frequencias tabela;
    nova_tabela_de_frequencias(&tabela);
    U8 byte;
    U64 tamanho_arquivo = 0;
    while (fread(&byte, 1, 1, f_in) == 1) {
        inclua_byte(byte, &tabela);
        tamanho_arquivo++;
    }
    rewind(f_in);

    // Passo 2: construir árvore de Huffman
    Ptr_de_no_de_arvore_binaria raiz = cria_arvore_huffman(&tabela);

    // Passo 3: gerar códigos
    Codigo codigos[256];
    for (int i = 0; i < 256; i++) novo_codigo(&codigos[i]);
    Codigo codigo_atual; novo_codigo(&codigo_atual);
    gera_codigos(raiz, codigos, &codigo_atual);
    free_codigo(&codigo_atual);

    // Passo 4: escrever tabela de frequências
    for (int i = 0; i < 256; i++) {
        U64 freq = tabela.vetor[i]
                  ? tabela.vetor[i]->informacao.frequencia
                  : 0;
        fwrite(&freq, sizeof(U64), 1, f_out);
    }

    // Reserva espaço p/ nº de bits e marca posição
    U64 quantidade_bits = 0;
    long pos_qtd_bits = ftell(f_out);
    fwrite(&quantidade_bits, sizeof(U64), 1, f_out);

    // Passo 5: codificar dados
    U8 buffer = 0;
    int buf_tam = 0;
    while (fread(&byte, 1, 1, f_in) == 1) {
        Codigo *cd = &codigos[byte];
        for (int b = 0; b < cd->tamanho; b++) {
            U8 valor = (cd->byte[b/8] >> (7 - (b%8))) & 1;
            buffer = (buffer << 1) | valor;
            buf_tam++;
            quantidade_bits++;
            if (buf_tam == 8) {
                fwrite(&buffer, 1, 1, f_out);
                buffer = buf_tam = 0;
            }
        }
    }
    if (buf_tam > 0) {
        buffer <<= (8 - buf_tam);
        fwrite(&buffer, 1, 1, f_out);
    }

    // Passo 6: volta e escreve a quantidade total de bits
    fseek(f_out, pos_qtd_bits, SEEK_SET);
    fwrite(&quantidade_bits, sizeof(U64), 1, f_out);

    // Cleanup
    fclose(f_in);
    fclose(f_out);
    for (int i = 0; i < 256; i++) free_codigo(&codigos[i]);

    printf("Compressão concluída: %s → %s\n", in_path, out_path);
    return EXIT_SUCCESS;
}
