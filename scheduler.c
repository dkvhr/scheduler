#include"types.h"
#include <stdlib.h>
#include<stdio.h>

Process *create_process(int id, int duration) {
        Process *new_proc;
        if ((new_proc = (Process *) malloc(sizeof(Process))) == NULL) {
                fprintf(stderr, "Nao foi possivel alocar memoria");
                exit(EXIT_FAILURE);
        }
        new_proc->pid = id;
        new_proc->duration = duration;
        new_proc->status = 0;
        return new_proc;
}

ProcessIO *create_IO_proc(int type, int duration, int priority) {
        ProcessIO *new_proc_IO;
        if ((new_proc_IO = (ProcessIO *) malloc(sizeof(ProcessIO))) == NULL) {
                fprintf(stderr, "Nao foi possivel alocar memoria");
                exit(EXIT_FAILURE);
        }
        new_proc_IO->type = type;
        new_proc_IO->duration = duration;
        new_proc_IO->priority = priority;
        return new_proc_IO;
}

/* Adicionando um processo comum a fila. Precisamos primeiro que o processo seja criado
para poder ser passado como parametro da funcao*/
void add_to_queue(Process *proc, Nodes *queue) {
        Nodes *new_node; // criando um novo node que vai ser o novo final da fila
        if((new_node = (Nodes *) malloc(sizeof(Nodes))) == NULL) {
                fprintf(stderr, "Nao foi possivel alocar memoria");
                exit(EXIT_FAILURE);
        }
        new_node->proc = *proc;
        new_node->next_proc = NULL;

        // indo ao final da fila e adicionando o node
        while(queue->next_proc != NULL) {
                queue = queue->next_proc;
        } 
}

/*
***Falta implementar

void change_queue(Queue **to, Queue *from) {
        Queue *temp;
        temp->proc = from->proc;
        while(from->next_proc != NULL) {
                from = from->next_proc;
        }
        new->next_proc = create_process();
        new->next_proc.process = (*prev)->process;
        *prev = (*prev)->process;
        
}
*/
int round_robin() {
        // Falta implementar
        return 0;
}