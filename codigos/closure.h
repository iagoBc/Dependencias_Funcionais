#ifndef CLOSURE_H
#define CLOSURE_H

#include "dfs.h"
#include <stdio.h>
#include <string.h>


// Função para calcular o fecho de um conjunto de atributos 
char* closure(DFS *dfs, const char *atrib_x);

#endif // CLOSURE_H