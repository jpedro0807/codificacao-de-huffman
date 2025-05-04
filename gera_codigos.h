#ifndef GERA_CODIGOS_H
#define GERA_CODIGOS_H

#include "tabela_de_frequencias.h"
#include "codigo.h"

// Gera os códigos de Huffman para cada byte, baseado na árvore.
// codigos[256] -> cada posição representa o código binário para o byte correspondente.
void gera_codigos(Ptr_de_no_de_arvore_binaria raiz,
                  Codigo codigos[256],
                  Codigo* codigo_atual);

#endif
