#ifndef MINCOVER_H
#define MINCOVER_H

#include "dfs.h"
#include <stdio.h>


// Função para calcular a cobertura mínima de um conjunto de dependências funcionais
int mincover(DFS *dfs);


// Função para decompor o lado direito das dependências funcionais em dependências unitárias
int decompor_rhs(DFS *dfs);


#endif // MINCOVER_H