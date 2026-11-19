#ifndef __QUEUE__
#define __QUEUE__

#include <stdio.h>
#include <stdlib.h>

// códigos de retorno das funções
#ifndef ERROR
#define ERROR -1
#define NOERROR 0
#endif


typedef struct No{
    void *dado;
    struct No *prox;
} No;


typedef struct queue_t{
    No *head;
    No *it;
    No *tail;
    int tam;
} queue_t;


// Função para criar uma fila
struct queue_t *queue_create();

// Função para destruir a fila
int queue_destroy(struct queue_t *queue);


// Função para adicionar um item na fila
int queue_add(struct queue_t *queue, void *item);


// Função para deletar um item da fila
int queue_del(struct queue_t *queue, void *item);


// Função para retornar o tamanho da fila
int queue_size(struct queue_t *queue);


// Função para retornar o item da cabeca da fila e colocar o iterador na cabeca
void *queue_head(struct queue_t *queue);


// Função para retornar o item apontado pelo iterador e avançar o iterador para o proximo no
void *queue_next(struct queue_t *queue);


#endif
