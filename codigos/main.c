#include "closure.h"
#include "dfs.h"
#include "mincover.h"
#include "keys.h"
#include "normalform.h"


int main(int argc, char *argv[]) {
    if(argc < 4){
        fprintf(stderr, "Uso: %s <closure|mincover|keys|normalform> --fds <arquivo>\n", argv[0]);
        return ERROR;
    }

    if(strcmp(argv[1], "closure") != 0 && strcmp(argv[1], "mincover") != 0 && strcmp(argv[1], "keys") != 0 && strcmp(argv[1], "normalform") != 0){
        fprintf(stderr, "Comando desconhecido: %s\n", argv[1]);
        return ERROR;
    }

    if(strcmp(argv[2], "--fds") != 0 || argv[3][0] == '\0'){
        fprintf(stderr, "Uso: %s <closure|mincover|keys|normalform> --fds <arquivo>\n", argv[0]);
        return ERROR;
    }

    if(strcmp(argv[1], "closure") == 0 &&
        (argc < 6 || strcmp(argv[4], "--X") != 0 || argv[5][0] == '\0')){
        fprintf(stderr, "Uso: %s closure --fds <arquivo> --X <atributos>\n", argv[0]);
        return ERROR;
    }

    DFS *dfs = preencher_dfs(argv[3]);
    if(dfs == NULL){
        fprintf(stderr, "Erro ao ler o arquivo de dependências funcionais.\n");
        return ERROR;
    }

    int status = 0;
    if(strcmp(argv[1], "closure") == 0){
        char *fecho = closure(dfs, argv[5]);
        if(fecho == NULL){
            fprintf(stderr, "Erro ao calcular o fecho.\n");
            status = ERROR;
        } 
        else{
            printf("%s\n", fecho);
            free(fecho);
        }
    }

    else if(strcmp(argv[1], "mincover") == 0){
        if(mincover(dfs) != NOERROR){
            fprintf(stderr, "Erro ao calcular a cobertura mínima.\n");
            status = ERROR;
        }
    }

    else if(strcmp(argv[1], "keys") == 0){
        queue_t* chaves = keys(dfs);
        if(chaves == NULL){
            fprintf(stderr, "Erro ao calcular as chaves candidatas.\n");
            status = ERROR;
        } 
        else{
            char* chave = queue_head(chaves);
            for(int i = 0; i < queue_size(chaves); i++){
                printf("%s\n", chave);
                chave = queue_next(chaves);
            }
            free(chaves);
        } 
        
    }

    else if(strcmp(argv[1], "normalform") == 0){
        // Implementação da funcionalidade de normalform
        if(normalform(dfs) != NOERROR) {
            fprintf(stderr, "Erro ao calcular a forma normal.\n");
            status = ERROR;
        }
    }

    return status;
}