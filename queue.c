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

NodeHead *create_node_head() {
        NodeHead *head;
        if ((head = (NodeHead *) malloc(sizeof(NodeHead))) == NULL) {
                fprintf(stderr, "Nao foi possivel alocar memoria");
                exit(EXIT_FAILURE);
        }

        head->front = NULL;
        head->rear = NULL;
        head->full_size = 0;

        return head;
}

int queue_is_empty(NodeHead *queue) {
        return queue->full_size == 0;
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
        queue->full_size++;
        return 0;
}

int node_head_dequeue(NodeHead *queue) {
        if(queue_is_empty(queue))
                return 1;

        ProcessNode *tmp;
        tmp = queue->front;
        queue->front = tmp->next_node;
        if(queue->full_size == 1)
                queue->rear = NULL;
        queue->full_size--;
        
        return 0;
}