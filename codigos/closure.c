#include "closure.h"
#include "dfs.h"

// Função para calcular o fecho de um conjunto de atributos
char* closure(DFS *dfs, const char *atrib_x){
    if (dfs == NULL || dfs->u == NULL || atrib_x == NULL) {
        return NULL;
    }

    char *fecho = malloc(strlen(dfs->u) + 1);
    if (fecho == NULL) {
        return NULL;
    }

    strcpy(fecho, atrib_x);                         // Inicializa o fecho com os atributos fornecidos

    int mudou;
    
    do{
        mudou = 0;
        Depend_func *dependencia = queue_head(dfs->f);
        while(dependencia != NULL){
            if(strstr(fecho, dependencia->lhs) != NULL &&
               strstr(fecho, dependencia->rhs) == NULL) {
                strcat(fecho, dependencia->rhs);
                mudou = 1;
            }

            dependencia = queue_next(dfs->f);
        }

    } while(mudou);

    return fecho;
}