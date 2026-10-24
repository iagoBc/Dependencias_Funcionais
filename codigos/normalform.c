#include "normalform.h"

int normalform(DFS *dfs){
    // Gere as Chaves: Execute a sua lógica de chaves candidatas validada anteriormente e armazene os resultados.
    queue_t* chaves = keys(dfs);
    chaves->tam--;
    if(chaves == NULL){
        fprintf(stderr, "Erro ao calcular as chaves candidatas.\n");
        return ERROR;
    }

    // Extraia Atributos Primos: Crie uma lista contendo apenas as letras que fazem parte de pelo menos uma chave candidata.
    char *primos = malloc(strlen(dfs->u) + 1);
    if(primos == NULL){
        fprintf(stderr, "Erro ao alocar memória para os atributos primos.\n");
        return ERROR;
    }
    primos[0] = '\0'; // Inicializa como string vazia de forma segura

    char *chave = queue_head(chaves);
    while(chave != NULL) {
        for(int i = 0; chave[i] != '\0'; i++){
            // Se o atributo da chave ainda não estiver na lista de primos, adiciona no final
            if(!strchr(primos, chave[i])){
                size_t tamanho = strlen(primos);
                primos[tamanho] = chave[i];
                primos[tamanho + 1] = '\0';
            }
        }
        chave = queue_next(chaves);
    }
    
    No *no = dfs->f->head;
    while(no != NULL){
        Depend_func *df = no->dado;
        char *fecho = closure(dfs, df->lhs);

        int regra_viola_bcnf = 0;
        int regra_viola_3fn = 0;

        // Verifica BCNF: o LHS é superchave?
        if(!contem_todos(fecho, dfs->u)){
            regra_viola_bcnf = 1;
            
            // Verifica 3FN no RHS (Lado Direito): todos os atributos do RHS são primos?
            if(!contem_parte(primos, df->rhs)) regra_viola_3fn = 1;
            
        }

        if(regra_viola_bcnf) printf("BCNF: VIOLATIONS\n");

        if(regra_viola_3fn) printf("3FN: VIOLATIONS\n");

        if(regra_viola_bcnf) printf("VIOLATION BCNF: %s->%s (%s not superkey)\n", df->lhs, df->rhs, df->lhs);

        if(regra_viola_3fn) printf("VIOLATION 3NF: %s->%s (%s not prime, %s not superkey)\n", df->lhs, df->rhs, df->rhs, df->lhs);
    

        free(fecho);
        no = no->prox;
    }

    return NOERROR;
}
