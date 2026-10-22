#include <stdio.h>
#include <string.h>
#include "closure.h"
#include "dfs.h"
#include "mincover.h"

int main(int argc, char *argv[]) {
    // 2. Verifica se o usuário digitou argumentos suficientes
    if(argc < 3){
        printf("Uso: fdtool <comando> [opcoes]\n");
        return 1;
    }

    DFS *dfs = preencher_dfs(argv[3]);
    
    if(!strcmp(argv[1], "closure")){
        if(argc < 5){
            printf("Uso: fdtool closure --fds <arquivo_fds> --X <atributos_X>\n");
            return 1;
        }

        printf("%s\n", closure(dfs, argv[5]));
    } 
    
    else if(!strcmp(argv[1], "mincover")){
        if(argc < 3){
            printf("Uso: fdtool mincover --fds <arquivo_fds>\n");
            return 1;
        }

        mincover(dfs);
    }


    else printf("Comando desconhecido: %s\n", argv[1]);
    
    queue_destroy(dfs->f);
    free(dfs->u);
    free(dfs);
    
    return 0;
}