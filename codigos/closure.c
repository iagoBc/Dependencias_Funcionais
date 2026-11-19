#include "closure.h"


// Função auxiliar para comparar dois caracteres (usada no qsort)
int comparar_atributos(const void *a, const void *b){
    return *(const char *)a - *(const char *)b;                                 // Retorna a diferença entre os dois caracteres, permitindo a ordenação em ordem crescente
}


// Função para verificar se as letras do conjunto 'menor' estão todas presentes no conjunto 'maior'
int contem_todos(const char *maior, const char *menor){
    for(int i = 0; menor[i] != '\0'; i++)
        if(!strchr(maior, menor[i])) return 0;                                  // Retorna falso se faltar alguma letra
        
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
        No *no = dfs->f->head;
        while(no != NULL){
            Depend_func *dependencia = no->dado;
            if(contem_todos(fecho, dependencia->lhs)){                          // Se o lado esquerdo da dependência funcional estiver contido no fecho, adiciona os atributos do lado direito ao fecho
                for(int i = 0; dependencia->rhs[i] != '\0'; i++){
                    if(!strchr(fecho, dependencia->rhs[i])){
                        size_t tam = strlen(fecho);
                        fecho[tam] = dependencia->rhs[i];
                        fecho[tam + 1] = '\0';
                        mudou = 1;
                    }
                }
            }
            no = no->prox;
        }
    } while(mudou);                                                             // Continua até que não haja mais mudanças no fecho                                

    qsort(fecho, strlen(fecho), sizeof(char), comparar_atributos);
    return fecho;
}