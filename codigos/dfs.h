#ifndef DFS_H
#define DFS_H

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

#include "queue.h"


typedef struct {
    char *lhs;          // Lado esquerdo (Left-Hand Side), ex: "A", "BD"
    char *rhs;          // Lado direito (Right-Hand Side), ex: "BC", "E"
} Depend_func;


typedef struct {
    char *u;              // Universo de atributos, ex: "ABCDE"
    queue_t *f;       // Fila de dependências funcionais
    int tam_f;         // 2. ESSENCIAL: Quantidade de regras cadastradas no array 'f'
} DFS;


// Função para preencher a estrutura DFS a partir de um arquivo
DFS* preencher_dfs(const char *arquivo);


#endif // DFS_H