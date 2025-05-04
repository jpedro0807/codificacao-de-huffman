#include <stdlib.h>
#include "huffman.h"
#include "lista_ordenada.h"

Ptr_de_no_de_arvore_binaria cria_arvore_huffman(Tabela_de_frequencias* tabela) {
    ListaOrdenada lista;
    nova_lista_ordenada(&lista);

    // Passo 1: Preenche lista ordenada com todos os nós não nulos da tabela de frequências
    for (int i = 0; i < 256; i++) {
        if (tabela->vetor[i] != NULL) {
            insere_ordenado(&lista, tabela->vetor[i]);
        }
    }

    // Passo 2: Enquanto houver mais de um nodo, combinar os dois menores
    while (lista.tamanho > 1) {
        Ptr_de_no_de_arvore_binaria nodo1 = remove_primeiro(&lista);
        Ptr_de_no_de_arvore_binaria nodo2 = remove_primeiro(&lista);

        // Criar novo nodo
        Ptr_de_no_de_arvore_binaria novo_nodo = (Ptr_de_no_de_arvore_binaria) malloc(sizeof(Struct_do_no_de_arvore_binaria));
        if (novo_nodo == NULL) return NULL; // erro de alocação

        novo_nodo->esquerda = nodo1;
        novo_nodo->direita = nodo2;

        // Byte inválido (0) porque é nodo interno
        novo_nodo->informacao.byte = 0;

        // Frequência combinada
        novo_nodo->informacao.frequencia = nodo1->informacao.frequencia + nodo2->informacao.frequencia;

        // Insere o novo nodo na lista ordenada
        insere_ordenado(&lista, novo_nodo);
    }

    // Quando sobrar um único nodo na lista → é a raiz da árvore de Huffman
    return remove_primeiro(&lista);
}
