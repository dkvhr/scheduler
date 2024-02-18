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
        node->prev_node = NULL;
        node->next_node = NULL;

        return node;
}

NodeHead *create_node_head() {
        NodeHead *head;
        if ((head = (NodeHead *) malloc(sizeof(NodeHead))) == NULL) {
                fprintf(stderr, "Nao foi possivel alocar memoria");
                exit(EXIT_FAILURE);
        }

        head->first_node = NULL;

        return head;
}