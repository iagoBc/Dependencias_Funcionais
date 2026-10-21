#include "closure.h"
#include "dfs.h"

// Função para calcular o fecho de um conjunto de atributos
void closure(DFS *dfs, const char *atrib_x){
    char fecho[strlen(dfs->u) + 1];
    strcpy(fecho, atrib_x);                         // Inicializa o fecho com os atributos fornecidos

    Depend_func *dependencia = queue_head(dfs->f);
    while(dependencia != NULL){
        if(strstr(fecho, dependencia->lhs) != NULL &&
           strstr(fecho, dependencia->rhs) == NULL) {
            strcat(fecho, dependencia->rhs);
        }

        dependencia = queue_next(dfs->f);
    }

    printf("%s\n", fecho);
}