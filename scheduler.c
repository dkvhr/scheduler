#include"types.h"
#include <stdlib.h>
#include<stdio.h>
#include<time.h>
#include<stdbool.h>

Process *create_process(int pid, int ppid, int duration, int activation_time, int num_of_IOs, NodeHead *processIOs) {
        Process *new_proc;
        if ((new_proc = (Process *) malloc(sizeof(Process))) == NULL) {
                fprintf(stderr, "Nao foi possivel alocar memoria");
                exit(EXIT_FAILURE);
        }
        new_proc->pid = pid;
        new_proc->ppid = ppid;
        new_proc->priority = 0; // procs novos entram na fila de prioridade alta
        new_proc->status = 0;
        new_proc->duration = duration;
        new_proc->total_exec = 0;
        new_proc->activation_time = activation_time;
        new_proc->num_of_IOs = num_of_IOs;
        // new_proc->processIOs = processIOs;

        return new_proc;
}

ProcessIO *create_IO_proc(int type, int priority) {
        ProcessIO *new_proc_IO;
        if ((new_proc_IO = (ProcessIO *) malloc(sizeof(ProcessIO))) == NULL) {
                fprintf(stderr, "Nao foi possivel alocar memoria");
                exit(EXIT_FAILURE);
        }
        new_proc_IO->type = type;
        switch (type) {
                case 0:
                        new_proc_IO->duration = DISK_TIME;
                        new_proc_IO->priority = 1;
                        break;
                case 1:
                        new_proc_IO->duration = TAPE_TIME;
                        new_proc_IO->priority = 0;
                        break;
                case 2:
                        new_proc_IO->duration = PRINTER_TIME;
                        new_proc_IO->priority = 0;
                        break;
        }
        return new_proc_IO;
}

/* Adicionando um processo comum a fila. Precisamos primeiro que o processo seja criado
para poder ser passado como parametro da funcao */

/*  ***FALTA AJEITAR ESSAS FUNCOES*** 
        O PROGRAMA NAO COMPILA AINDA
*/
void add_to_queue(Process *proc, ProcessNode *queue) {
        ProcessNode *new_node; // criando um novo node que vai ser o novo final da fila
        if((new_node = (ProcessNode *) malloc(sizeof(ProcessNode))) == NULL) {
                fprintf(stderr, "Nao foi possivel alocar memoria");
                exit(EXIT_FAILURE);
        }
        new_node->proc = *proc;
        new_node->next_proc = NULL;

        // indo ao final da fila e adicionando o node
        // talvez seja melhor fazer de alguma forma mais eficiente
        while(queue->next_proc != NULL) {
                queue = queue->next_proc;
        } 
        queue->next_proc = new_node;
}

// troca de fila entre processos normais
void change_queue(ProcessNode *origin, ProcessNode **destination) {
        ProcessNode *temp;
        temp->proc = origin->proc;
        // indo ao final da fila de destino
        while((* destination)->next_proc != NULL) {
                (* destination) = (* destination)->next_proc;
        }
        new->next_proc = create_process();
        new->next_proc.process = (*prev)->process;
        *prev = (*prev)->process;
        
}

int round_robin() {
        // Falta implementar
        time_t start = time(NULL);
        int procs_left = 0;



        return 0;
}