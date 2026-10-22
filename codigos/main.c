#include "closure.h"
#include "dfs.h"
#include "mincover.h"


int main(int argc, char *argv[]) {
    if(argc < 4){
        fprintf(stderr, "Uso: %s <closure|mincover> --fds <arquivo>\n", argv[0]);
        return 1;
    }

    if(strcmp(argv[1], "closure") != 0 && strcmp(argv[1], "mincover") != 0){
        fprintf(stderr, "Comando desconhecido: %s\n", argv[1]);
        return 1;
    }

    if(strcmp(argv[2], "--fds") != 0 || argv[3][0] == '\0'){
        fprintf(stderr, "Uso: %s <closure|mincover> --fds <arquivo>\n", argv[0]);
        return 1;
    }

    if(strcmp(argv[1], "closure") == 0 &&
        (argc < 6 || strcmp(argv[4], "--X") != 0 || argv[5][0] == '\0')){
        fprintf(stderr, "Uso: %s closure --fds <arquivo> --X <atributos>\n", argv[0]);
        return 1;
    }

    DFS *dfs = preencher_dfs(argv[3]);
    if(dfs == NULL){
        fprintf(stderr, "Erro ao ler o arquivo de dependências funcionais.\n");
        return 1;
    }

    int status = 0;
    if(strcmp(argv[1], "closure") == 0){
        char *fecho = closure(dfs, argv[5]);
        if(fecho == NULL){
            fprintf(stderr, "Erro ao calcular o fecho.\n");
            status = 1;
        } 
        else{
            printf("%s\n", fecho);
            free(fecho);
        }
    }

    else if(mincover(dfs) != NOERROR){
        fprintf(stderr, "Erro ao calcular a cobertura mínima.\n");
        status = 1;
    }

    return status;
}