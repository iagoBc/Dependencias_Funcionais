#include <stdio.h>
#include <string.h>
#include "closure.h"
#include "dfs.h"

int main(int argc, char *argv[]) {
    // 2. Verifica se o usuário digitou argumentos suficientes
    if(argc < 3){
        printf("Uso: fdtool <comando> [opcoes]\n");
        return 1;
    }

    DFS *dfs = preencher_dfs(argv[3]);
    
    if(!strcmp(argv[1], "closure")) {
        // 4. Se for "closure", chama a função calcular_closure com os argumentos apropriados
        if(argc < 5) {
            printf("Uso: fdtool closure --fds <arquivo_fds> --X <atributos_X>\n");
            return 1;
        }
        closure(dfs, argv[5]);
    } 
    
    else {
        printf("Comando desconhecido: %s\n", argv[1]);
    }


    return 0;
}