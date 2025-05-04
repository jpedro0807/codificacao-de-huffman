#ifndef HUFFMAN_H
#define HUFFMAN_H

#include "tabela_de_frequencias.h"
#include "lista_ordenada.h"

// Cria a árvore de Huffman com base na tabela de frequências.
// Retorna ponteiro para o nó raiz da árvore de Huffman.
Ptr_de_no_de_arvore_binaria cria_arvore_huffman(Tabela_de_frequencias* tabela);

#endif

