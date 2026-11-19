#include "mincover.h"
#include "closure.h"


// Função para ordenar as dependências funcionais em ordem lexicográfica
void ordenar_dependencias(queue_t *fila){
    if(queue_size(fila) < 2) return;

    int mudou;
    do{
        mudou = 0;
        No *no = fila->head;

        while(no != NULL && no->prox != NULL){
            Depend_func *atual = no->dado;
            Depend_func *proximo = no->prox->dado;

            int lhs_maior = strcmp(atual->lhs, proximo->lhs) > 0;
            int lhs_igual = strcmp(atual->lhs, proximo->lhs) == 0;

            if(lhs_maior || (lhs_igual && strcmp(atual->rhs, proximo->rhs) > 0)){
                no->dado = proximo;
                no->prox->dado = atual;
                mudou = 1;
            }

            no = no->prox;
        }
    } while(mudou);
}


// Função para decompor o lado direito das dependências funcionais em dependências unitárias
int decompor_rhs(DFS *dfs){
    No *no = dfs->f->head;
    while(no != NULL){
        No *prox = no->prox;                                                    // Salva o próximo antes de qualquer remoção
        Depend_func *df = no->dado;

        if(strlen(df->rhs) > 1){                                                // Se o lado direito da dependência funcional tiver mais de um atributo, decompõe em dependências unitárias
            for(size_t i = 0; i < strlen(df->rhs); i++){                        // Para cada atributo no lado direito, cria uma nova dependência funcional unitária
                Depend_func *new_df = malloc(sizeof(*new_df));
                if(new_df == NULL) return ERROR;

                new_df->lhs = malloc(strlen(df->lhs) + 1);
                new_df->rhs = malloc(2);
                if(!new_df->lhs || !new_df->rhs){
                    free(new_df->lhs);
                    free(new_df->rhs);
                    free(new_df);
                    return ERROR;
                }

                strcpy(new_df->lhs, df->lhs);                                   // Copia o lado esquerdo da dependência funcional original para a nova dependência
                new_df->rhs[0] = df->rhs[i];
                new_df->rhs[1] = '\0';
                queue_add(dfs->f, new_df);
            }

            queue_del(dfs->f, df);                                              // Remove a dependência funcional original da fila
            free(df->lhs);
            free(df->rhs);
            free(df);
        }
        no = prox;
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

    int tam_total = queue_size(dfs->f);
    for(int i = 0; i < tam_total; i++){                                             // 3. Não possuir dependências redundantes
        Depend_func *df = queue_head(dfs->f);
        queue_del(dfs->f, df);                                                      // Remove o primeiro elemento da fila para testá-lo
        
        char *fecho = closure(dfs, df->lhs);                                        // Calcula o fecho usando as dependências que restaram na fila
        if(!fecho) return ERROR;

        if(strstr(fecho, df->rhs) != NULL){                                         // O RHS já é determinado pelas outras regras.  
            free(df->lhs);
            free(df->rhs);
            free(df);
        }                               
        else queue_add(dfs->f, df);                                                 // Adiciona a regra de volta (ela irá para o final da fila)
        free(fecho);
    }

    ordenar_dependencias(dfs->f);
    return NOERROR;                                                           
}
