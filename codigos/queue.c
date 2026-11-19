#include "queue.h"


// Função para criar uma fila
struct queue_t *queue_create(){
    queue_t *fila = (queue_t*) malloc(sizeof(queue_t));                         // Aloca dinamicamente memoria para struct

    if(!fila) return NULL;                          
    
    else{                                                                       // Inicia os valores dos nos e o tamanho = 0
        fila->head = NULL;
        fila->it = NULL;
        fila->tail = NULL;
        fila->tam = 0;
        return fila;
    }
}


// Função para destruir a fila
int queue_destroy(struct queue_t *queue){
    if(!queue) return ERROR;
    
    No *prox = NULL;

    while(queue->head){                                                         // Enquanto tiver um no na fila
        prox = queue->head->prox;
        free(queue->head);
        queue->head = prox;                                                     // Nova cabeca da fila
    } 

    free(queue);

    return NOERROR;
}


// Função para adicionar um item na fila
int queue_add(struct queue_t *queue, void *item){
    if(!queue || !item) return ERROR;                                           // Retorna ERROR se a fila ou item nao existe

    No* no = (No*) malloc(sizeof(No));
    if (!no) return ERROR;

    no->prox = NULL;                                                            
    no->dado = item;                                                            // O valor dentro do no = item

    if(!queue->tam)                                                             // Se a fila esta vazia
        queue->head = queue->tail = queue->it = no;
    

    else{ 
        queue->tail->prox = no;                                                 // Relaciona o valor da calda com o novo no
        queue->tail = no;                                                       // Nova calda
    }

    queue->tam++;                                                               

    return NOERROR;
}


// Função para deletar um item da fila
int queue_del(struct queue_t *queue, void *item){
    if(!queue || !item) return ERROR;                                           // Retorna ERROR se a fila ou item nao existe

    No* no = queue->head;                                                       
    No* anterior = NULL;                                                       

    while(no && no->dado != item){                                              // Enquanto dado do no != item e no != NULL
        anterior = no;                                                          // Anterior guarda o valor do no
        no = no->prox;                                                          // No vira o proximo no da fila
    }

    if(!no) return ERROR;                                                       // Se no = NULL nao existe o item na fila ou a fila = vazia

    if(queue->tam == 1) queue->head = queue->it = queue->tail = NULL;           // Se a fila tem apenas um no
    else if(queue->head == no) queue->head = no->prox;                          // Se o no a ser deletado = cabeca da fila
    else anterior->prox = no->prox;                                             // Deleta o no e liga o no anterior com o proximo no

    if(!no->prox) queue->tail = anterior;                                       // Se o no deletado = calda da fila, atualiza a calda
    if(queue->it == no) queue->it = no->prox;                                   // Se o no deletado esta apontado pelo iterador, o iterador vai para o prox no

    free(no); 

    queue->tam--; 

    return NOERROR;
}


// Função para retornar o tamanho da fila
int queue_size(struct queue_t *queue){
    if(!queue) return ERROR; 
    else return queue->tam;
}


// Função para retornar o item da cabeca da fila e colocar o iterador na cabeca
void *queue_head(struct queue_t *queue){
    if (!queue || !queue->head) return NULL;                                    // Retorna ERROR se a fila ou item nao existe

    queue->it = queue->head;                                                    // Coloca o iterador na cabeca da fila

    return queue->it->dado;                                                     // Retorna para o item apontado pelo iterador
}


// Função para retornar o item apontado pelo iterador e avançar o iterador para o proximo no
void *queue_next(struct queue_t *queue){
    if (!queue || !queue->it) return NULL;                                      // Retorna ERROR se a fila ou item nao existe

    queue->it = queue->it->prox;                                                // Avanca o iterador ao proximo no da fila
    if (!queue->it) return NULL;                                                // Se o proximo no == NULL

    return queue->it->dado;                                                     // Retorna para o item apontado pelo iterador
}
