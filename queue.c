#include "types.h"
#include<stdlib.h>
#include<stdio.h>

/* Aqui implementei uma queue usando linked list*/

ProcessNode *create_process_node(Process *proc) {
        ProcessNode *node;
        if ((node = (ProcessNode *) malloc(sizeof(ProcessNode))) == NULL) {
                fprintf(stderr, "Nao foi possivel alocar memoria");
                exit(EXIT_FAILURE);
        }

        node->proc = proc;
        node->next_node = NULL;

        return node;
}

NodeHead *create_node_head(int max_size, int priority) {
        NodeHead *head;
        if ((head = (NodeHead *) malloc(sizeof(NodeHead))) == NULL) {
                fprintf(stderr, "Nao foi possivel alocar memoria");
                exit(EXIT_FAILURE);
        }

        head->front = NULL;
        head->rear = NULL;
        head->full_size = max_size;
        head->priority = priority;
        head->size = 0;

        return head;
}

int queue_is_empty(NodeHead *queue) {
        return queue->size == 0;
}

int node_head_enqueue(NodeHead *queue, Process *proc) {
        ProcessNode *tmp;
        if ((tmp = (ProcessNode *) malloc(sizeof(ProcessNode))) == NULL) {
                fprintf(stderr, "Nao foi possivel alocar memoria");
                exit(EXIT_FAILURE);
        }

        tmp->proc = proc;
        tmp->queue_time = 0;
        tmp->next_node = NULL;
        if (queue_is_empty(queue)) {
                queue->front = tmp;
                queue->rear = tmp;
        }

        else {
                queue->rear->next_node = tmp;
                queue->rear = tmp;
        }
        queue->size++;
        return 0;
}

Process *node_head_dequeue(NodeHead *queue) {
        // remove o primeiro elemento da fila e retorna como Process
        if(queue_is_empty(queue)) {
                queue->front = queue->rear = NULL;
                return NULL;
        }

        ProcessNode *tmp;
        tmp = queue->front;
        queue->front = tmp->next_node;
        if(queue->size == 1)
                queue->rear = NULL;
        queue->size--;
        if(queue_is_empty(queue)) {
                queue->front = queue->rear = NULL;
                return tmp->proc;
        }
        
        return tmp->proc;
}