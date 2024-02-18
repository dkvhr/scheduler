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

ProcessIO *create_IO_proc(int type, int activation_time) {
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
        new_proc_IO->activation_time = activation_time;
        return new_proc_IO;
}

/* Adicionando um processo comum a fila. Precisamos primeiro que o processo seja criado
para poder ser passado como parametro da funcao */

// troca de fila entre processos normais
/*void change_queue(ProcessNode *origin, ProcessNode **destination) {
        ProcessNode *temp;
        temp->proc = origin->proc;
        // indo ao final da fila de destino
        while((* destination)->next_proc != NULL) {
                (* destination) = (* destination)->next_proc;
        }
        new->next_proc = create_process();
        new->next_proc.process = (*prev)->process;
        *prev = (*prev)->process;
        
}*/

ProcessIO *check_IO_request(Process *proc) {
        if(proc->IO_req->size == 0) {
                printf("Nao ha processos IO, continuando...\n");
                return NULL;
        }

        for(int i=0; i < proc->IO_req->size; ++i) {
                if(proc->IO_req->request[i]->activation_time == ((proc->total_exec) - (proc->remaining_time))) {
                        printf("Temos uma IO request do tipo %d\n", proc->IO_req->request[i]->type);
                        return proc->IO_req->request[i];
                }
                printf("Existe uma IO request numero %d mas ela nao deve ser executada no momento\n", i);
        }
                return NULL;
}

RoundRobin round_robin(int q, int num_of_IOs) {
        // Falta implementar
        RoundRobin rr;
        rr.quantum = q;
        rr.time_elapsed = 0;
        rr.max_procs = MAX_PROCS;
        rr.executing_proc = NULL;
        // criando filas de procs e IO
        rr.high_priority = create_node_head();
        rr.low_priority = create_node_head();
        rr.IO_queue = (NodeHead **) malloc(num_of_IOs*sizeof(NodeHead *));
        rr.IO_queue[0] = create_node_head(); //Disk
        rr.IO_queue[1] = create_node_head(); //Tape
        rr.IO_queue[2] = create_node_head(); //Printer

        return rr;
}