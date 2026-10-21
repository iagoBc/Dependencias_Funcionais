#include "closure.h"
#include "dfs.h"

// Função para calcular o fecho de um conjunto de atributos
void closure(DFS *dfs, const char *atrib_x){
    char fecho[strlen(dfs->u) + 1];
    strcpy(fecho, atrib_x);                         // Inicializa o fecho com os atributos fornecidos

    for(int i = 0; i < dfs->tam_f; i++){
        if(strstr(fecho, dfs->f[i].lhs) != NULL && strstr(fecho, dfs->f[i].rhs) == NULL)
            strcat(fecho, dfs->f[i].rhs);           // Se o lado esquerdo da dependência funcional estiver no fecho e o lado direito não estiver, adiciona o lado direito ao fecho
    }

    printf("%s\n", fecho);
}