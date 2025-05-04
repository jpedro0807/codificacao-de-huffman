#ifndef LISTA_ORDENADA_H
#define LISTA_ORDENADA_H
#include "myTypesClion.h"
#include "tabela_de_frequencias.h"

typedef struct ListaOrdenadaNo {
    Ptr_de_no_de_arvore_binaria nodo;
    struct ListaOrdenadaNo* prox;
} ListaOrdenadaNo;

typedef struct {
    ListaOrdenadaNo* primeiro;
    U16 tamanho;
} ListaOrdenada;

void nova_lista_ordenada(ListaOrdenada* lista);
boolean insere_ordenado(ListaOrdenada* lista, Ptr_de_no_de_arvore_binaria nodo);
Ptr_de_no_de_arvore_binaria remove_primeiro(ListaOrdenada* lista);
boolean lista_vazia(ListaOrdenada* lista);
void libera_lista(ListaOrdenada* lista);

#endif


