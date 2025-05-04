#include "lista_ordenada.h"
#include <stdlib.h>


void nova_lista_ordenada(ListaOrdenada* lista) {
    lista->primeiro = NULL;
    lista->tamanho = 0;
}

boolean insere_ordenado(ListaOrdenada* lista, Ptr_de_no_de_arvore_binaria nodo) {
    ListaOrdenadaNo* novo = (ListaOrdenadaNo*) malloc(sizeof(ListaOrdenadaNo));
    if (novo == NULL) return false;

    novo->nodo = nodo;
    novo->prox = NULL;

    // Inserir no lugar certo
    if (lista->primeiro == NULL || nodo->informacao.frequencia < lista->primeiro->nodo->informacao.frequencia) {
        novo->prox = lista->primeiro;
        lista->primeiro = novo;
    } else {
        ListaOrdenadaNo* atual = lista->primeiro;
        while (atual->prox != NULL && atual->prox->nodo->informacao.frequencia <= nodo->informacao.frequencia) {
            atual = atual->prox;
        }
        novo->prox = atual->prox;
        atual->prox = novo;
    }

    lista->tamanho++;
    return true;
}

Ptr_de_no_de_arvore_binaria remove_primeiro(ListaOrdenada* lista) {
    if (lista->primeiro == NULL) return NULL;

    ListaOrdenadaNo* removido = lista->primeiro;
    Ptr_de_no_de_arvore_binaria nodo = removido->nodo;

    lista->primeiro = removido->prox;
    free(removido);
    lista->tamanho--;

    return nodo;
}

boolean lista_vazia(ListaOrdenada* lista) {
    return lista->primeiro == NULL;
}

void libera_lista(ListaOrdenada* lista) {
    while (!lista_vazia(lista)) {
        remove_primeiro(lista);
    }
}
