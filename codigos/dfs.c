#include "dfs.h"
#include <ctype.h>
#include <stdio.h>


// Função para remover espaços em branco de uma string
void remover_espacos(char *texto){
    char *origem = texto;
    char *destino = texto;

    while(*origem != '\0'){
        if(!isspace((unsigned char)*origem)){                                       // Se o caractere atual não for um espaço em branco, copia-o para a posição de destino
            *destino = *origem;
            destino++;                                                  
        }
        origem++;                                               
    }

    *destino = '\0';                                                    
}


// Função para preencher a estrutura DFS a partir de um arquivo
DFS* preencher_dfs(const char *arquivo){
    FILE *fp = fopen(arquivo, "r");
    if(fp == NULL){
        perror(arquivo);
        return NULL;
    } 

    DFS *dfs = malloc(sizeof(*dfs));
    if(dfs == NULL){
        fclose(fp);
        return NULL;
    }

    dfs->u = NULL;
    dfs->f = NULL;
    dfs->tam_f = 0;

    char linha[1024];
    while(fgets(linha, sizeof(linha), fp) != NULL){
        char *inicio = linha;
        remover_espacos(inicio);                                                    

        if(*inicio == 'U'){
            char *abre = strchr(inicio+1, '{');                                     
            if(!abre) continue;
            char *fecha = strchr(abre + 1, '}');                                    
            if(!fecha) continue;

            size_t quantidade = 0;
            for(char *atrib = abre + 1; atrib < fecha; atrib++) 
                if(isalpha((unsigned char)*atrib)) quantidade++;                    // Conta a quantidade de atributos alfabéticos entre '{' e '}'
            

            dfs->u = malloc(quantidade + 1);                                        // Aloca memória para o universo de atributos, incluindo espaço para o caractere nulo
            if(dfs->u == NULL){
                fclose(fp);
                free(dfs);
                return NULL;
            }

            size_t pos = 0;
            for(char *atrib = abre + 1; atrib < fecha; atrib++)
                if(isalpha((unsigned char)*atrib)) dfs->u[pos++] = *atrib;          // Copia os atributos alfabéticos para o universo de atributos
                
            dfs->u[pos] = '\0';                                         
        }
        
        if(*inicio == 'F'){
            char *abre = strchr(inicio + 1, '{');
            if(!abre) continue;
            char *fecha = strchr(abre + 1, '}');
            if(!fecha) continue;

            dfs->tam_f = 0;
            for(char *p = abre + 1; *p != '\0'; p++) 
                if(*p == ',' || *p == ';') dfs->tam_f++;                            // Conta a quantidade de dependências funcionais entre '{' e '}'
            

            if(*(abre + 1) != '\0') dfs->tam_f++;                                   // Incrementa a contagem se houver pelo menos uma dependência funcional

            dfs->f = queue_create();
            if(dfs->f == NULL){
                fclose(fp);
                free(dfs);
                return NULL;
            }

            char *dependencias[dfs->tam_f];                                         // Array para armazenar as dependências funcionais temporariamente
            dependencias[0] = strtok(abre + 1, ",;}");
            for(int i = 0; i < dfs->tam_f && dependencias[i] != NULL; i++)
                if(i+1 < dfs->tam_f) dependencias[i+1] = strtok(NULL, ",;}");
            

            for(int i = 0; i < dfs->tam_f; i++){
                Depend_func *df = malloc(sizeof(*df));

                if(df == NULL){
                    fclose(fp);
                    free(dfs);
                    return NULL;
                }

                df->lhs = strtok(dependencias[i], "->");                            // Separa o lado esquerdo da dependência funcional usando "->" como delimitador
                df->rhs = strtok(NULL, "->");                                       // Separa o lado direito da dependência funcional usando "->" como delimitador
                queue_add(dfs->f, df);                                              // Adiciona a dependência funcional à fila de dependências funcionais
            } 
        }
    }

    // Imprime o universo de atributos armazenado na estrutura DFS
    printf("U = {");
    for(size_t i = 0; i < strlen(dfs->u); i++){
        if(i == strlen(dfs->u)-1) printf("%c}\n", dfs->u[i]);
        else printf("%c,", dfs->u[i]);
    } 

    // Imprime as dependências funcionais armazenadas na fila
    printf("F = {");
    Depend_func *df = queue_head(dfs->f);
    int primeira = 1;

    while(df != NULL){
        if(!primeira) printf(", ");
        printf("%s->%s", df->lhs, df->rhs);
        primeira = 0;
        df = queue_next(dfs->f);
    }
    printf("}\n");

    fclose(fp);
    return dfs;
}
