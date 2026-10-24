#include "keys.h"

// Função auxiliar para verificar se todos os atributos do lado esquerdo estão presentes no fecho
int contem_parte(char *maior, char *menor){
    for(int i = 0; menor[i] != '\0'; i++)
        if(!strchr(maior, menor[i])) return 0;                                    // Retorna falso se faltar alguma letra
        
    return 1;                                                                   // Verdadeiro se todas estiverem no fecho
}

queue_t* keys(DFS *dfs){
    queue_t *chaves = queue_create();                       
    int pos = 0;
    int tam_u = strlen(dfs->u);

    long long num_combinacoes = 1LL << tam_u;                                                   // Calcula o número total de combinações possíveis de chaves candidatas 2 ^ tam_u

    for(long long i = 1; i < num_combinacoes; i++){
        char *combinacao = malloc(tam_u + 1);                                                                 // Aloca memória para a combinação atual
        if(combinacao == NULL){
            fprintf(stderr, "Erro ao alocar memória para a combinação.\n");
            return NULL;
        }

        int index = 0;
        for(int j = 0; j < tam_u; j++)
            if(i & (1LL << j)) combinacao[index++] = dfs->u[j];                                 // Adiciona o atributo correspondente à combinação atual
            
        combinacao[index] = '\0';
        char *chave = queue_head(chaves);

        for(int j = 0; j < pos; j++){
            if(contem_parte(combinacao, chave)){                                         // Se a combinação atual contiver uma chave candidata já encontrada, ela não é uma chave candidata
                free(combinacao);
                combinacao = NULL;
                break;
            }
            chave = queue_next(chaves);
        }

        if(combinacao == NULL) continue;

        char *fecho = closure(dfs, combinacao);
        if(fecho == NULL){
            fprintf(stderr, "Erro ao calcular o fecho da combinação %s.\n", combinacao);
            free(combinacao);
            return NULL;
        }

        if(contem_todos(fecho, dfs->u)){
            queue_add(chaves, combinacao);
            pos++;
        }
        
        else free(combinacao);
        free(fecho);
    }

    return chaves;
}