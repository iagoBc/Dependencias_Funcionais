#ifndef CLOSURE_H
#define CLOSURE_H

#include "dfs.h"


// Função para calcular o fecho de um conjunto de atributos 
char* closure(DFS *dfs, const char *atrib_x);


// Função para verificar se o fecho contém todos os atributos do universo
int contem_todos(const char *fecho, const char *atributos);


#endif // CLOSURE_H