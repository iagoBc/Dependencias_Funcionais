#include "mincover.h"
#include "closure.h"


// Função para decompor o lado direito das dependências funcionais em dependências unitárias
int decompor_rhs(DFS *dfs){
    Depend_func *df = queue_head(dfs->f);
    while(df != NULL){
        if(strlen(df->rhs) > 1){                                                    // Se o lado direito da dependência funcional tiver mais de um atributo, cria novas dependências funcionais para cada atributo do lado direito
            for(size_t i = 0; i < strlen(df->rhs); i++){
                Depend_func *new_df = malloc(sizeof(*new_df));          
                if(new_df == NULL){
                    free(df);
                    return ERROR;
                }
                new_df->lhs = df->lhs;
                new_df->rhs = malloc(2);                
                new_df->rhs[0] = df->rhs[i];
                new_df->rhs[1] = '\0';                                  
                queue_add(dfs->f, new_df);                                          // Adiciona a nova dependência funcional à fila de dependências funcionais
            }
            queue_del(dfs->f, df);
        }
        df = queue_next(dfs->f);
    }

    return NOERROR;
}


// Função para calcular a cobertura mínima das dependências funcionais
int mincover(DFS *dfs){
    if(decompor_rhs(dfs) != NOERROR) return ERROR;                                  // 1. Decompor o lado direito das dependências funcionais em dependências unitárias

    Depend_func *df = queue_head(dfs->f);                                           // 2. Não possuir atributos desnecessários nas dependências
    while(df != NULL){
        if(strlen(df->lhs) > 1){                                                    // Se o lado esquerdo da dependência funcional tiver mais de um atributo, verifica se algum deles é desnecessário
            for(size_t i = 0; i < strlen(df->lhs); i++){
                size_t tam_lhs = strlen(df->lhs);
                char *lhs_reduzido = malloc(tam_lhs);
                if(lhs_reduzido == NULL) return ERROR;

                size_t pos = 0;
                for(size_t j = 0; j < tam_lhs; j++)
                    if(j != i) lhs_reduzido[pos++] = df->lhs[j];                    // Cria uma nova string com o lado esquerdo da dependência funcional sem o atributo candidato
                lhs_reduzido[pos] = '\0';

                char *fecho = closure(dfs, lhs_reduzido);                           // Calcula o fecho sem o atributo candidato usando as dependências atuais
                if(fecho == NULL){
                    free(lhs_reduzido);
                    return ERROR;
                }

                if(strstr(fecho, df->rhs) != NULL){                                 // Se o fecho contiver o RHS, o atributo candidato é desnecessário
                    free(df->lhs);
                    df->lhs = lhs_reduzido;
                    free(fecho);
                    i = (size_t)-1;
                    continue;
                }

                free(lhs_reduzido);
                free(fecho);
            }
        }

        df = queue_next(dfs->f);
    }
    
    for(int i = 0; i < dfs->tam_f; i++){                                            // 3. Não possuir dependências redundantes
        Depend_func *df = queue_head(dfs->f);
        queue_del(dfs->f, df);                                                      // Remove o primeiro elemento da fila para testá-lo
        
        char *fecho = closure(dfs, df->lhs);                                        // Calcula o fecho usando as dependências que restaram na fila
        if(strstr(fecho, df->rhs) != NULL) free(df);                                // O RHS já é determinado pelas outras regras.  
        else queue_add(dfs->f, df);                                                 // Adiciona a regra de volta (ela irá para o final da fila)
        free(fecho);
    }

    return NOERROR;                                                           
}
