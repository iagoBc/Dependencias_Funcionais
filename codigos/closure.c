#include "closure.h"
#include "dfs.h"


// Função auxiliar para verificar se todos os atributos do lado esquerdo estão presentes no fecho
int contem_todos_atributos(const char *fecho, const char *lhs){
    for(int i = 0; lhs[i] != '\0'; i++)
        if(!strchr(fecho, lhs[i])) return 0;                        // Retorna falso se faltar alguma letra
        
    return 1;                                                       // Verdadeiro se todas estiverem no fecho
}


// Função para calcular o fecho de um conjunto de atributos
char* closure(DFS *dfs, const char *atrib_x){
    if(dfs == NULL || dfs->u == NULL || atrib_x == NULL) return NULL;

    char *fecho = malloc(strlen(dfs->u) + 1);
    if(fecho == NULL) return NULL;
    
    for(size_t i = 0; i < strlen(atrib_x); i++){
        if(strchr(dfs->u, atrib_x[i]) == NULL){                      // Verifica se o atributo atual está presente no universo de atributos
            printf("Erro: Atributo '%c' não pertence ao universo de atributos.\n", atrib_x[i]);
            free(fecho);
            return NULL;
        }
    }
    
    strcpy(fecho, atrib_x);                                         // Inicializa o fecho com os atributos fornecidos
    int mudou;
    
    do{
        mudou = 0;
        Depend_func *dependencia = queue_head(dfs->f);              // Obtém a primeira dependência funcional da fila
        while(dependencia != NULL){
            if(contem_todos_atributos(fecho, dependencia->lhs)){    // Verifica se todos os atributos do lado esquerdo da dependência funcional estão presentes no fecho
                // Adiciona apenas os caracteres do RHS que ainda não estão no fecho
                for(int i = 0; dependencia->rhs[i] != '\0'; i++){
                    if(strchr(fecho, dependencia->rhs[i]) == NULL){ // Verifica se o caractere do RHS já está presente no fecho
                        size_t tam = strlen(fecho);
                        fecho[tam] = dependencia->rhs[i];     
                        fecho[tam + 1] = '\0';
                        mudou = 1;                                   // Indica que houve uma mudança no fecho
                    }
                }
            }
            dependencia = queue_next(dfs->f);
        }
    } while(mudou);                                                 // Repete o processo até que não haja mais mudanças no fecho

    return fecho;
}