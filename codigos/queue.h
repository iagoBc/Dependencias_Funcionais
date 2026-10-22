#ifndef __QUEUE__
#define __QUEUE__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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


// Cria uma fila inicialmente vazia.
struct queue_t *queue_create();


// Destroi uma fila, liberando a memória alocada por ela.
int queue_destroy(struct queue_t *queue);


// Adiciona um item no fim da fila.
int queue_add(struct queue_t *queue, void *item);


// Retira da fila o item com o valor indicado.
int queue_del(struct queue_t *queue, void *item);


// Informa o número de itens na fila.
int queue_size(struct queue_t *queue);


// Põe o iterador no início da fila.
void *queue_head(struct queue_t *queue);


// Avança o iterador ao próximo item na fila.
void *queue_next(struct queue_t *queue);


#endif
