#include <stdlib.h>
#include "tabela_de_frequencias.h"

void nova_tabela_de_frequencias(Tabela_de_frequencias* tab)
{
    for (U16 i = 0; i < 256; i++)
        tab->vetor[i] = NULL;

    tab->quantidade_de_posicoes_preenchidas = 0;
}

static boolean novo_no_de_arvore_binaria(
    Ptr_de_no_de_arvore_binaria* novo,
    Ptr_de_no_de_arvore_binaria esq,
    Elemento inf,
    Ptr_de_no_de_arvore_binaria dir
) {
    *novo = (Ptr_de_no_de_arvore_binaria)malloc(sizeof(Struct_do_no_de_arvore_binaria));
    if (*novo == NULL) return false;

    (*novo)->esquerda = esq;
    (*novo)->informacao = inf;
    (*novo)->direita = dir;

    return true;
}

boolean inclua_byte(U8 byte, Tabela_de_frequencias* tab)
{
    if (tab->vetor[byte] != NULL) {
        tab->vetor[byte]->informacao.frequencia++;
        return true;
    }

    Elemento elem;
    elem.byte = byte;
    elem.frequencia = 1;

    boolean OK = novo_no_de_arvore_binaria(&(tab->vetor[byte]), NULL, elem, NULL);
    if (!OK) return false;

    tab->quantidade_de_posicoes_preenchidas++;
    return true;
}

void junte_nodos_no_inicio_do_vetor(Tabela_de_frequencias* tab)
{
    U16 primeiro_NULL, primeiro_nao_NULL;

    for (;;) {
        primeiro_NULL = 0;
        while (primeiro_NULL < 256 && tab->vetor[primeiro_NULL] != NULL)
            primeiro_NULL++;
        if (primeiro_NULL == 256) break;

        primeiro_nao_NULL = primeiro_NULL + 1;
        while (primeiro_nao_NULL < 256 && tab->vetor[primeiro_nao_NULL] == NULL)
            primeiro_nao_NULL++;
        if (primeiro_nao_NULL == 256) break;

        tab->vetor[primeiro_NULL] = tab->vetor[primeiro_nao_NULL];
        tab->vetor[primeiro_nao_NULL] = NULL;
    }
}
