#ifndef KEYS_H
#define KEYS_H

#include "dfs.h"
#include "closure.h"
#include "queue.h"

// Função para calcular chaves candidatas de um conjunto de dependências funcionais
queue_t* keys(DFS *dfs);

#endif // KEYS_H