#include "mincover.h"
#include "closure.h"

void mincover(DFS *dfs){
    Depend_func *df = queue_head(dfs->f);

    // 1. Possuir apenas um atributo no lado direito de cada dependência
    while(df != NULL){
        if(strlen(df->rhs) > 1){                                        // Se o lado direito da dependência funcional tiver mais de um atributo, cria novas dependências funcionais para cada atributo do lado direito
            for(size_t i = 0; i < strlen(df->rhs); i++){
                Depend_func *new_df = malloc(sizeof(*new_df));          
                if(new_df == NULL){
                    free(df);
                    return;
                }
                new_df->lhs = df->lhs;
                new_df->rhs = malloc(2);                                // Aloca memória para o lado direito da nova dependência funcional (1 caractere + '\0')
                new_df->rhs[0] = df->rhs[i];
                new_df->rhs[1] = '\0';                                  
                queue_add(dfs->f, new_df);                              // Adiciona a nova dependência funcional à fila de dependências funcionais
            }
            queue_del(dfs->f, df);
        }
        df = queue_next(dfs->f);
    }

    // 2. Não possuir atributos desnecessários nas dependências
    df = queue_head(dfs->f);
    while(df != NULL){
        if(strlen(df->lhs) > 1){                                        // Se o lado esquerdo da dependência funcional tiver mais de um atributo, verifica se algum deles é desnecessário
            for(size_t i = 0; i < strlen(df->lhs); i++){
                char atrib_x[2] = {df->lhs[i], '\0'};                   // Cria uma string temporária com o atributo atual do lado esquerdo
                char *fecho = closure(dfs, atrib_x);                    // Calcula o fecho do atributo atual usando as dependências funcionais restantes na fila
                if(strstr(fecho, df->rhs) != NULL){ 
                    df->lhs = atrib_x;
                    break;
                }
                free(fecho);
            }
        }

        df = queue_next(dfs->f);
    }
    
    // 3. Não possuir dependências redundantes
    for(int i = 0; i < queue_size(dfs->f); i++){
        Depend_func *df = queue_head(dfs->f);
        queue_del(dfs->f, df);                                          // Remove o primeiro elemento da fila para testá-lo
        
        char *fecho = closure(dfs, df->lhs);                            // Calcula o fecho usando as dependências que restaram na fila
        if(strstr(fecho, df->rhs) != NULL) free(df);                    // Redundante: O RHS já é determinado pelas outras regras.
        
        else queue_add(dfs->f, df);                                     // Essencial: Adiciona a regra de volta (ela irá para o final da fila).
        free(fecho);
    }

    df = queue_head(dfs->f);
    while(df != NULL){
        printf("%s->%s\n", df->lhs, df->rhs);
        df = queue_next(dfs->f);
    }
}
