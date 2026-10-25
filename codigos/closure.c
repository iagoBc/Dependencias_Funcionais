#include "closure.h"
#include "dfs.h"


// Função para verificar se as letras do conjunto 'menor' estão todas presentes no conjunto 'maior'
int contem_todos(const char *maior, const char *menor){
    for(int i = 0; menor[i] != '\0'; i++)
        if(!strchr(maior, menor[i])) return 0;                                    // Retorna falso se faltar alguma letra
        
    return 1;                                                                   // Verdadeiro se todas estiverem no fecho
}


// Função para calcular o fecho de um conjunto de atributos
char* closure(DFS *dfs, const char *atrib_x){
    if(!dfs || !dfs->u || !atrib_x) return NULL;

    char *fecho = malloc(strlen(dfs->u) + 1);
    if(!fecho) return NULL;
    
    for(size_t i = 0; i < strlen(atrib_x); i++){
        if(!strchr(dfs->u, atrib_x[i])){                                        // Verifica se o atributo atual está presente no universo de atributos
            fprintf(stderr, "Erro: Atributo '%c' não pertence ao universo de atributos.\n", atrib_x[i]);
            free(fecho);
            return NULL;
        }
    }
    
    strcpy(fecho, atrib_x);        
    int mudou;
    
    do{
        mudou = 0;
        Depend_func *dependencia = queue_head(dfs->f);                      
        while(dependencia != NULL){
            if(contem_todos(fecho, dependencia->lhs)){                          // Verifica se todos os atributos do lado esquerdo da dependência funcional estão presentes no fecho
                // Adiciona apenas os caracteres do RHS que ainda não estão no fecho
                for(int i = 0; dependencia->rhs[i] != '\0'; i++){
                    if(!strchr(fecho, dependencia->rhs[i])){                    // Verifica se o caractere do RHS já está presente no fecho
                        size_t tam = strlen(fecho);                             
                        fecho[tam] = dependencia->rhs[i];                       // Adiciona o caractere do RHS ao final do fecho
                        fecho[tam + 1] = '\0';
                        mudou = 1;      
                    }
                }
            }
            dependencia = queue_next(dfs->f);
        }
    } while(mudou);                                                             // Repete o processo até que não haja mais mudanças no fecho

    return fecho;
}