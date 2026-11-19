#include "keys.h"


// Função auxiliar para verificar se todos os atributos do lado esquerdo estão presentes no fecho
int contem_parte(char *maior, char *menor){
    for(int i = 0; menor[i] != '\0'; i++)
        if(!strchr(maior, menor[i])) return 0;                                          // Retorna falso se faltar alguma letra

    return 1;                                                                           // Verdadeiro se todas estiverem no fecho
}


// Função para encontrar todas as chaves candidatas do conjunto de dependências funcionais
queue_t* keys(DFS *dfs){
    queue_t *chaves = queue_create();
    int pos = 0;
    int tam_u = strlen(dfs->u);

    int *obrigatorios = calloc(tam_u, sizeof(*obrigatorios));
    if(!obrigatorios) return NULL;

    for(int i = 0; i < tam_u; i++){                                                     // Identifica atributos que devem estar em toda chave candidata
        int aparece_no_rhs = 0;
        Depend_func *dependencia = queue_head(dfs->f);

        while(dependencia != NULL){
            if(strchr(dependencia->rhs, dfs->u[i]) != NULL){
                aparece_no_rhs = 1;
                break;
            }
            dependencia = queue_next(dfs->f);
        }

        if(!aparece_no_rhs) obrigatorios[i] = 1;                                        // Se o atributo não aparece no rhs de nenhuma df, ele é obrigatório em toda chave candidata

    }

    long long num_combinacoes = 1LL << tam_u;                                           // Calcula o número total de combinações possíveis de chaves candidatas 2 ^ tam_u

    for(long long i = 1; i < num_combinacoes; i++){
        char *combinacao = malloc(tam_u + 1);                                           // Aloca memória para a combinação atual
        if(!combinacao) return NULL;

        int index = 0;
        for(int j = 0; j < tam_u; j++)
            if(i & (1LL << j)) combinacao[index++] = dfs->u[j];                         // Adiciona o atributo correspondente à combinação atual

        combinacao[index] = '\0';
        int possui_obrigatorio = 1;
        for(int j = 0; j < tam_u; j++){
            if(obrigatorios[j] && !strchr(combinacao, dfs->u[j])){                      // Verifica se a combinação atual contém todos os atributos obrigatórios
                possui_obrigatorio = 0;
                break;
            }
        }
        if(!possui_obrigatorio){
            free(combinacao);
            continue;
        }

        char *chave = queue_head(chaves);

        for(int j = 0; j < pos; j++){
            if(contem_parte(combinacao, chave)){                                        // Se a combinação atual contiver uma chave candidata já encontrada, ela não é uma chave candidata
                free(combinacao);
                combinacao = NULL;
                break;
            }
            chave = queue_next(chaves);
        }

        if(!combinacao) continue;

        char *fecho = closure(dfs, combinacao);
        if(!fecho){
            fprintf(stderr, "Erro ao calcular o fecho da combinação %s.\n", combinacao);
            free(combinacao);
            free(obrigatorios);
            return NULL;
        }

        if(contem_todos(fecho, dfs->u)){                                                // Se o fecho da combinação atual contiver todos os atributos do universo, ela é uma chave candidata
            queue_add(chaves, combinacao);
            pos++;
        }

        else free(combinacao);
        free(fecho);
    }

    free(obrigatorios);
    return chaves;
}