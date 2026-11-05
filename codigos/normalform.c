#include "normalform.h"

// Função para calcular a forma normal de um conjunto de dependências funcionais
int normalform(DFS *dfs){
    queue_t* chaves = keys(dfs);
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
    primos[0] = '\0';                                                                            // Inicializa como string vazia de forma segura

    char *chave = queue_head(chaves);

    while(chave != NULL){                                                                        // Identificar os atributos primos
        for(int i = 0; chave[i] != '\0'; i++){
            if(!strchr(primos, chave[i])){                                                       // Se o atributo da chave ainda não estiver na lista de primos, adiciona no final
                size_t tamanho = strlen(primos);
                primos[tamanho] = chave[i];
                primos[tamanho + 1] = '\0';
            }
        }
        chave = queue_next(chaves);
    }
    
    int geral_viola_bcnf = 0;
    int geral_viola_3fn = 0;

    // Primeiro loop: apenas verifica
    No *no = dfs->f->head;

    while(no != NULL){
        Depend_func *df = no->dado;
        char *fecho = closure(dfs, df->lhs);

        if(!contem_todos(fecho, dfs->u)){                                                        // Verifica BCNF: o LHS é superchave? 
            geral_viola_bcnf = 1;
            if(!contem_parte(primos, df->rhs)) geral_viola_3fn = 1;                              // Verifica 3FN no RHS (Lado Direito): todos os atributos do RHS são primos?
        }

        free(fecho);
        no = no->prox;
    }

    if(geral_viola_bcnf) printf("BCNF: VIOLATIONS\n");
    if(geral_viola_3fn) printf("3FN: VIOLATIONS\n");

    no = dfs->f->head;
    while(no != NULL){
        Depend_func *df = no->dado;
        char *fecho = closure(dfs, df->lhs);

        int regra_viola_bcnf = 0;
        int regra_viola_3fn = 0;

        if(!contem_todos(fecho, dfs->u)){
            regra_viola_bcnf = 1;                                                                 // Verifica BCNF: o LHS é superchave?
            if(!contem_parte(primos, df->rhs)) regra_viola_3fn = 1;                               // Verifica 3FN no RHS (Lado Direito): todos os atributos do RHS são primos?
        } 
        
        if(regra_viola_bcnf) printf("VIOLATION BCNF: %s->%s (%s not superkey)\n", df->lhs, df->rhs, df->lhs);
        if(regra_viola_3fn) printf("VIOLATION 3NF: %s->%s (%s not prime, %s not superkey)\n", df->lhs, df->rhs, df->rhs, df->lhs);
    
        free(fecho);
        no = no->prox;
    }

    return NOERROR;
}
