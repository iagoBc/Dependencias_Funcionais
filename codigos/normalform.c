#include "normalform.h"


// Dependência trivial: RHS ⊆ LHS (nunca viola BCNF nem 3NF)
int trivial(Depend_func *df){
    return contem_todos(df->lhs, df->rhs);
}


// Função para calcular a forma normal de um conjunto de dependências funcionais
int normalform(DFS *dfs){
    queue_t* chaves = keys(dfs);                                                                // Calcula as chaves candidatas
    if(!chaves) return ERROR;
    
    if(decompor_rhs(dfs) != NOERROR) return ERROR;                                              // Decompor o lado direito das dependências funcionais em dependências unitárias

    chaves->tam--;
    if(!chaves){
        fprintf(stderr, "Erro ao calcular as chaves candidatas.\n");
        return ERROR;
    }

    char *primos = malloc(strlen(dfs->u) + 1);
    if(!primos){
        fprintf(stderr, "Erro ao alocar memória para os atributos primos.\n");
        return ERROR;
    }
    primos[0] = '\0';                                                                           // Inicializa como string vazia de forma segura

    char *chave = queue_head(chaves);

    while(chave != NULL){                                                                       // Identificar os atributos primos
        for(int i = 0; chave[i] != '\0'; i++){
            if(!strchr(primos, chave[i])){                                                      // Se o atributo da chave ainda não estiver na lista de primos, adiciona no final
                size_t tamanho = strlen(primos);
                primos[tamanho] = chave[i];
                primos[tamanho + 1] = '\0';
            }
        }
        chave = queue_next(chaves);
    }
    
    int viola_bcnf = 0;
    int viola_3nf = 0;

    // Primeiro loop: apenas verifica
    No *no = dfs->f->head;
    while(no != NULL){
        Depend_func *df = no->dado;
        char *fecho = closure(dfs, df->lhs);

        if(!trivial(df) && !contem_todos(fecho, dfs->u)){                                       // Verifica BCNF: o LHS é superchave? 
            viola_bcnf = 1;
            if(!contem_parte(primos, df->rhs)) viola_3nf = 1;                                   // Verifica 3FN no RHS (Lado Direito): todos os atributos do RHS são primos?
        }

        free(fecho);
        no = no->prox;
    }

    if(viola_bcnf) printf("BCNF: VIOLATIONS\n");
    else printf("BCNF: OK\n");
    if(viola_3nf) printf("3NF: VIOLATIONS\n");
    else printf("3NF: OK\n");

    // Segundo loop: imprime as violações de BCNF
    no = dfs->f->head;
    while(no != NULL){
        Depend_func *df = no->dado;
        char *fecho = closure(dfs, df->lhs);

        if(!trivial(df) && !contem_todos(fecho, dfs->u)) 
            printf("VIOLATION BCNF: %s->%s (%s not superkey)\n", df->lhs, df->rhs, df->lhs);    // Verifica BCNF: o LHS é superchave?
    
        free(fecho);
        no = no->prox;
    }


    // Terceiro loop: imprime as violações de 3NF
    no = dfs->f->head;
    while(no != NULL){
        Depend_func *df = no->dado;
        char *fecho = closure(dfs, df->lhs);

        if(!trivial(df) && !contem_todos(fecho, dfs->u)){
            if(!contem_parte(primos, df->rhs))                                                  // Verifica 3FN no RHS (Lado Direito): todos os atributos do RHS são primos? 
                printf("VIOLATION 3NF: %s->%s (%s not prime, %s not superkey)\n", 
                    df->lhs, df->rhs, df->rhs, df->lhs);                                        
        } 
    
        free(fecho);
        no = no->prox;
    }

    free(primos);
    
    return NOERROR;
}
