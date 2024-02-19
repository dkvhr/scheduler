#include"types.h"
#include <stdlib.h>
#include<stdio.h>
#include<time.h>
#include<stdbool.h>

// Declaracoes de funcoes
int rr_process_add(RoundRobin *rr, ProcessList *process_list);
void rr_checkIO(RoundRobin *rr);
void rr_execute_process(RoundRobin *rr);
void rr_end_process(int pid, ProcessList *proc_list);


Process *create_process(int pid, int ppid, int duration, int activation_time, int num_of_IOs) {
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

RoundRobin round_robin_init() {
        // Falta implementar
        RoundRobin rr;
        rr.time_elapsed = 0; // tempo relativo a cada vez que o proc e executado (dps de uma preempcao ele volta pra 0)
        rr.max_procs = MAX_PROCS;
        rr.executing_proc = NULL;
        // criando filas de procs e IO
        rr.high_priority = create_node_head();
        rr.low_priority = create_node_head();
        rr.IO_queue = (NodeHead **) malloc(3 * sizeof(NodeHead *));
        rr.IO_queue[0] = create_node_head(); //Disk
        rr.IO_queue[1] = create_node_head(); //Tape
        rr.IO_queue[2] = create_node_head(); //Printer

        return rr;
}

void rr_next_action(RoundRobin *rr, ProcessList *proc_list) {
        ProcessIO *io_req;
        io_req = NULL;
        // proc sendo executado no momento:
        if(rr->executing_proc != NULL) {
                io_req = check_IO_request(rr->executing_proc);
                rr->executing_proc->remaining_time--;
                rr->time_elapsed++;
        }

        rr_process_add(rr, proc_list);
        rr_checkIO(rr);

        if(rr->executing_proc == NULL) {
                printf("Nada sendo executado no momento\n");
                rr_execute_process(rr);
        }
        else if(rr->executing_proc->remaining_time == 0) {
                printf("Processo [%d] finalizado\n", rr->executing_proc->pid);
                rr_end_process(rr->executing_proc->pid, proc_list);

                rr_execute_process(rr);
        }
        else if((rr->time_elapsed) == QUANTUM) {
                printf("Processo [%d] atingiu o tempo de quantum e sofrera preempcao", rr->executing_proc->pid);
                rr->executing_proc->status = 0;
                node_head_enqueue(rr->low_priority, rr->executing_proc);
                rr_execute_process(rr);
        }
        else if(io_req != NULL) {
                rr->executing_proc->status = 2;
                switch (io_req->type) {
                        case 0:
                                node_head_enqueue(rr->IO_queue[0], rr->executing_proc);
                                break;
                        case 1:
                                node_head_enqueue(rr->IO_queue[1], rr->executing_proc);
                                break;
                        case 2:
                                node_head_enqueue(rr->IO_queue[2], rr->executing_proc);
                                break;
                }
                rr_execute_process(rr);
        }
}

int rr_process_add(RoundRobin *rr, ProcessList *process_list) {
        if(process_list == NULL) {
                fprintf(stderr, "Lista de processos para entrar esta vazia\n");
                return 1;
        }

        for(int i=0; i<process_list->size; ++i) {
                if(process_list->procs[i] != NULL && (process_list->procs[i]->arrival_time == rr->time_elapsed)) {
                        printf("Processo com PID: %d sendo adicionado ao escalonador\n", process_list->procs[i]->pid);
                        node_head_enqueue(rr->high_priority, process_list->procs[i]); // sempre que um proc eh adicionado ele deve entrar na fila de alta prioridade
                }
        }
        return 0;
}

void rr_checkIO(RoundRobin *rr) {
        for(int i=0; i<3; ++i) {
                if(rr->IO_queue[i]->front != NULL) {
                        rr->IO_queue[i]->front->queue_time++;
                        if (rr->IO_queue[i]->front->queue_time == rr->IO_queue[i]->priority) {
                                Process *tmp = node_head_dequeue(rr->IO_queue[i]);
                                tmp->status = 0;
                                switch (i) {
                                        case 0:
                                                node_head_enqueue(rr->low_priority, tmp);
                                                break;
                                        case 1:
                                                node_head_enqueue(rr->high_priority, tmp);
                                                break;
                                        case 2:
                                                node_head_enqueue(rr->high_priority, tmp);
                                                break;
                                
                                }
                        
                        }
                }
        }
}


void rr_execute_process(RoundRobin *rr) {
        rr->executing_proc = NULL;
        rr->time_elapsed = 0;

        Process *proc;
        if((proc = node_head_dequeue(rr->high_priority)) == NULL) {
                printf("Fila de alta prioridade vazia\n");
                if((proc = node_head_dequeue(rr->low_priority)) == NULL) {
                        printf("Fila de baixa prioridade vazia!\n");
                        exit(EXIT_FAILURE);
                }
        }
        proc->status = 1;
        rr->executing_proc = proc;
}

void rr_end_process(int pid, ProcessList *proc_list) {
        for(int i=0; i<proc_list->size; ++i) {
                if((proc_list->procs[i]) != NULL && (proc_list->procs[i]->pid == pid)) {
                        proc_list->procs[i] = NULL;
                }
        }
}

int rr_check_end_of_processes(RoundRobin *rr, ProcessList *proc_list) {
        if(rr->executing_proc != NULL){
                printf("Processo existente em execucao\n");
                return 1;
        }

        if(rr->low_priority->front != NULL) {
                printf("Processo existente em execucao na fila de baixa prioridade\n");
                return 1;
        }

        if(rr->high_priority->front != NULL) {
                printf("Processo existente em execucao na fila de alta prioridade\n");
                return 1;
        }

        for(int i=0; i<proc_list->size; ++i) {
                if(proc_list->procs[i] != NULL) {
                        printf("Processo existente na fila de processos \n");
                        return 1;
                }
        }

        for(int i=0; i<3; ++i) {
                if(rr->IO_queue[i]->front != NULL) {
                        printf("Processo existente na fila de IO \n");
                        return 1;
                }
        }
        return 0;
}