#ifndef CLOSURE_H
#define CLOSURE_H

#include "dfs.h"


// Função para calcular o fecho de um conjunto de atributos 
char* closure(DFS *dfs, const char *atrib_x);


// Função para verificar se as letras do conjunto 'menor' estão todas presentes no conjunto 'maior'
int contem_todos(const char *maior, const char *menor);


#endif // CLOSURE_H