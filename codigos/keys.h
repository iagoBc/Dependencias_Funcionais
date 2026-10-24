#ifndef KEYS_H
#define KEYS_H

#include "dfs.h"
#include "closure.h"
#include "queue.h"

// Função para calcular chaves candidatas de um conjunto de dependências funcionais
queue_t* keys(DFS *dfs);

// Função auxiliar para verificar se todos os atributos do lado esquerdo estão presentes no fecho
int contem_parte(char *maior, char *menor);
#endif // KEYS_H