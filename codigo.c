#include "codigo.h"
#include <stdlib.h>

boolean novo_codigo (Codigo* c /* por referência */)
{
    // Inicializa o código com um byte já zerado
    c->byte = (U8*)malloc(sizeof(U8));
    if (c->byte == NULL) return false;
    c->byte[0] = 0;

    c->capacidade = 8;  // em bits
    c->tamanho   = 0;  // em bits
    return true;
}

void free_codigo (Codigo* c /* por referência */)
{
    if (c->byte != NULL) {
        free(c->byte);
        c->byte = NULL;
    }
    c->capacidade = 0;
    c->tamanho   = 0;
}

boolean adiciona_bit (Codigo* c /* por referência */, U8 valor /* 0 ou 1 */)
{
    // Se atingiu a capacidade, expande em mais 8 bits (1 byte)
    if (c->tamanho == c->capacidade) {
        size_t old_bytes = c->capacidade / 8;
        U8* novo = (U8*)realloc(c->byte, (old_bytes + 1) * sizeof(U8));
        if (novo == NULL) return false;
        c->byte = novo;
        // Zera o novo byte alocado
        c->byte[old_bytes] = 0;
        c->capacidade += 8;
    }

    // Posiciona o bit no byte e na posição correta (MSB first)
    int byte_pos = c->tamanho / 8;
    int bit_pos  = 7 - (c->tamanho % 8);

    if (valor)
        c->byte[byte_pos] |=  (1 << bit_pos);
    else
        c->byte[byte_pos] &= ~(1 << bit_pos);

    c->tamanho++;
    return true;
}

boolean joga_fora_bit (Codigo* c /* por referência */)
{
    if (c->tamanho == 0) return false;

    // Decrementa tamanho e limpa o bit correspondente
    c->tamanho--;
    int byte_pos = c->tamanho / 8;
    int bit_pos  = 7 - (c->tamanho % 8);
    c->byte[byte_pos] &= ~(1 << bit_pos);

    // Se sobrar um byte inteiro sem uso, reduz a capacidade
    if (c->capacidade > 8 && (c->capacidade - c->tamanho) >= 8) {
        size_t new_bytes = (c->capacidade / 8) - 1;
        U8* novo = (U8*)realloc(c->byte, new_bytes * sizeof(U8));
        if (novo == NULL) return false;
        c->byte = novo;
        c->capacidade -= 8;
    }

    return true;
}

boolean clone (Codigo original, Codigo* copia /* por referência */)
{
    // Copia apenas os bytes realmente usados (tamanho em bits arredondado para cima)
    int total_bytes = (original.tamanho + 7) / 8;
    copia->byte = (U8*)malloc(total_bytes * sizeof(U8));
    if (copia->byte == NULL) return false;

    for (int i = 0; i < total_bytes; i++)
        copia->byte[i] = original.byte[i];

    copia->capacidade = total_bytes * 8;
    copia->tamanho   = original.tamanho;
    return true;
}
