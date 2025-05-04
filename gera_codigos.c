#include <stdlib.h>  // ou #include <stddef.h>
#include "gera_codigos.h"

// Função recursiva para gerar os códigos da árvore de Huffman.
void gera_codigos(Ptr_de_no_de_arvore_binaria raiz,
                  Codigo codigos[256],
                  Codigo* codigo_atual)
{
    if (raiz == NULL) return;

    // Se for um nó folha
    if (raiz->esquerda == NULL && raiz->direita == NULL) {
        clone(*codigo_atual, &codigos[raiz->informacao.byte]);
        return;
    }

    adiciona_bit(codigo_atual, 0);
    gera_codigos(raiz->esquerda, codigos, codigo_atual);
    joga_fora_bit(codigo_atual);

    adiciona_bit(codigo_atual, 1);
    gera_codigos(raiz->direita, codigos, codigo_atual);
    joga_fora_bit(codigo_atual);
}
