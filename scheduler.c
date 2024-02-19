#include"types.h"
#include <stdlib.h>
#include<stdio.h>
#include<time.h>
#include<stdbool.h>

// Declaracoes de funcoes
void rr_run_all_before_preemption(RoundRobin *rr);

Process *create_process(int pid, int ppid, unsigned duration, unsigned arrival_time, IORequest *io_req) {
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
        new_proc->arrival_time = arrival_time;
        new_proc->remaining_time = duration;
        new_proc->IO_req = io_req;

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

IORequest *create_IO_request(ProcessIO **req, int size) {
        IORequest *io_req;
        if((io_req = (IORequest *) malloc(sizeof(IORequest *))) == NULL) {
                fprintf(stderr, "Nao foi possivel alocar memoria\n");
                exit(EXIT_FAILURE);
        }
        io_req->request = req;
        io_req->size = size;

        return io_req;
}

NodeHead *rr_waiting_for_io(int io_type, RoundRobin *rr) {
        switch(io_type) {
                case 0:
                        return rr->IO_queue[0];
                case 1:
                        return rr->IO_queue[1];
                case 2:
                        return rr->IO_queue[2];
        }
        return NULL;
}

RoundRobin round_robin_init() {
        RoundRobin rr;
        rr.time_elapsed = 0; // tempo relativo a cada vez que o proc e executado (dps de uma preempcao ele volta pra 0)
        rr.quantum = 0;
        rr.max_procs = MAX_PROCS;
        rr.active_processes = 0;
        rr.running_procs = NULL;
        // criando filas de procs e IO
        rr.new_procs = create_node_head(MAX_PROCS, 0);
        rr.high_priority = create_node_head(MAX_PROCS, 0);
        rr.low_priority = create_node_head(MAX_PROCS, 1);
        rr.IO_queue = (NodeHead **) malloc(3 * sizeof(NodeHead *));
        rr.IO_queue[0] = create_node_head(MAX_PROCS, 1); //Disk
        rr.IO_queue[1] = create_node_head(MAX_PROCS, 0); //Tape
        rr.IO_queue[2] = create_node_head(MAX_PROCS, 0); //Printer
        rr.finished_procs = create_node_head(MAX_PROCS, 0);
        rr.IO_proc_queue = create_node_IO_head(MAX_IO_PROCS, MAX_IO_PROCS);

        rr_run_all_before_preemption(&rr);

        return rr;
}

int rr_is_running_proc(RoundRobin *rr) {
        return rr->running_procs != NULL;
}

int rr_has_active_processes(RoundRobin *rr) {
        return rr->active_processes > 0;
}

int rr_start_quantum(RoundRobin *rr) {
        return rr->quantum == 0;
}

void print_queues(RoundRobin *rr) {
        ProcessNode *tmp = (ProcessNode *) malloc(sizeof(ProcessNode));
        if(tmp == NULL) {
                fprintf(stderr, "Erro ao alocar memoria\n");
                exit(EXIT_FAILURE);
        }
        printf("A fila de alta prioridade contem os processos:\n");
        printf("[%d] ", rr->high_priority->front->proc->pid);
        tmp = rr->high_priority->front;
        while(tmp != NULL) {
                printf("[%d] ", tmp->proc->pid);
                tmp = tmp->next_node;
        }
        printf("\n\nA fila de baixa prioridade contem os processos:\n");
        printf("[%d] ", rr->low_priority->front->proc->pid);
        tmp = rr->low_priority->front;
        while(tmp != NULL) {
                printf("[%d] ", tmp->proc->pid);
                tmp = tmp->next_node;
        }
        printf("\n\n");
        printf("A fila de IO se encontra com %d processos. Sendo dos tipos:\n", rr->IO_proc_queue->size);
        ProcessIONode *io_tmp = (ProcessIONode *) malloc(sizeof(ProcessIONode));
        if(io_tmp == NULL) {
                fprintf(stderr, "Erro ao alocar memoria\n");
                exit(EXIT_FAILURE);
        }
        io_tmp = rr->IO_proc_queue->front;
        printf("Tipo %d, ", io_tmp->procIO->type);
        while(io_tmp != NULL) {
                io_tmp = io_tmp->next_node;
                printf("%d, ", io_tmp->procIO->type);
        }
        printf("\n");
}

int rr_running_to_wait(RoundRobin *rr) {
        node_head_enqueue(rr_waiting_for_io(rr->running_procs->io_type, rr), rr->running_procs);
        rr->running_procs->status = 2;
        rr->running_procs->time_waiting = 0;
        rr->running_procs = NULL;
        return 0;
}

void rr_finish_running_proc(RoundRobin *rr) {
        int turnaround = 0;
        node_head_enqueue(rr->finished_procs, rr->running_procs);
        rr->running_procs->status = 3;
        rr->running_procs = NULL;
        turnaround = rr->time_elapsed - rr->running_procs->arrival_time;
        printf("O processo [%d] terminou!\n", rr->running_procs->pid);
        printf("Seu turnaround e de: %d\n", turnaround);
        rr->active_processes--;
}

void rr_pass_time(RoundRobin *rr) {
        rr->time_elapsed++;
        rr->quantum++;
        if(rr->quantum == QUANTUM) {
                rr->quantum = 0; // reseta e sofre preempcao
                printf("O processo %d sofreu preempcao no tempo %d!\n", rr->running_procs->pid, rr->time_elapsed);
        }
}

int rr_running_to_ready(RoundRobin *rr) {
        if(!rr_is_running_proc(rr))
                return 1;
        if(!rr_start_quantum(rr))
                return 1;
        int new_priority = 1;
        rr->running_procs->priority = new_priority;
        node_head_enqueue(rr->low_priority, rr->running_procs);
        rr->running_procs->status = 0;
        rr->running_procs = NULL;
        return 0;
}

int rr_best_process(RoundRobin *rr) {
        if(!queue_is_empty(rr->high_priority))
                return 0;
        if(!queue_is_empty(rr->low_priority))
                return 1;
        return 2;
}

int rr_ready_to_running(RoundRobin *rr) {
        int priority;
        if(rr_is_running_proc(rr))
                return 1;
        if(!rr_start_quantum(rr))
                return 1;
        priority = rr_best_process(rr);
        if(priority == 2)
                return 1;
        if(priority == 0)
                rr->running_procs = rr->high_priority->front->proc;
        if(priority == 1)
                rr->running_procs = rr->low_priority->front->proc;
        rr->running_procs->status = 1;
        if(priority == 0)
                node_head_dequeue(rr->high_priority);
        else
                node_head_dequeue(rr->low_priority);
        printf("Processo [%d] admitido!\n", rr->running_procs->pid);
        return 0;

}

void rr_add_new_proc(RoundRobin *rr) {
        while(!queue_is_empty(rr->new_procs) && rr->active_processes < MAX_PROCS) {
                Process *proc = rr->new_procs->front->proc;
                node_head_enqueue(rr->high_priority, proc);
                proc->status = 0;
                proc->priority = 0;
                node_head_dequeue(rr->new_procs);
                rr->active_processes++;
        }
}

int rr_waiting_to_ready(RoundRobin *rr) {
        for(int i=0; i<3; ++i) {
                NodeHead *waiting_queue = rr_waiting_for_io(i, rr);
                Process *proc;
                if(queue_is_empty(waiting_queue))
                        continue;
                proc = waiting_queue->front->proc;
                int duration, new_priority;
                switch(proc->io_type) {
                        case 0:
                                duration = 2;
                                new_priority = 1;
                                break;
                        case 1:
                                duration = 4;
                                new_priority = 0;
                                break;
                        case 2:
                                duration = 6;
                                new_priority = 0;
                                break;
                }
                if(proc->time_waiting == duration) {
                        if(new_priority == 0)
                                node_head_enqueue(rr->high_priority, proc);
                        if(new_priority == 1)
                                node_head_enqueue(rr->low_priority, proc);
                        proc->status = 0;
                        proc->priority = new_priority;
                        node_head_dequeue(waiting_queue);
                }
        }
        return 0;
}

void rr_run_proc(RoundRobin *rr) {
        if(!rr_is_running_proc(rr))
                return;
        rr->running_procs->remaining_time--;
        printf("Processo [%d] sendo executado. Tempo restante: %d\n", rr->running_procs->pid, rr->running_procs->remaining_time);
        if(rr->running_procs->remaining_time == 0)
                rr_finish_running_proc(rr);
}

void rr_io_finish_running_proc(RoundRobin *rr) {
        int turnaround = 0;
        printf("Processo de IO do tipo %d terminou no tempo %d", rr->io_running_procs->type, rr->time_elapsed);
        IO_node_head_dequeue(rr->IO_proc_queue);
        rr->io_running_procs = NULL;
        turnaround = rr->time_elapsed - rr->io_running_procs->activation_time;
        printf("Seu turnaround e de: %d\n", turnaround);
        rr->active_processes--;
}

void rr_run_IO_proc(RoundRobin *rr) {
        if(!rr_is_running_proc(rr))
                return;
        rr->IO_proc_queue->front->procIO->remaining_time--;
        printf("Processo de IO do tipo %d sendo executado. Tempo restante: %d\n", rr->IO_proc_queue->front->procIO->type, rr->IO_proc_queue->front->procIO->remaining_time);
        if(rr->IO_proc_queue->front->procIO->remaining_time == 0)
                rr_io_finish_running_proc(rr);
}

void rr_pass_time_waiting_proc(RoundRobin *rr) {
        for(int i=0; i<3; i++) {
                NodeHead *waiting_queue = rr_waiting_for_io(i, rr);
                if(queue_is_empty(waiting_queue))
                        continue;
                waiting_queue->front->proc->time_waiting++;
        }
}

void rr_run_all_after_preemption(RoundRobin *rr) {
        rr_pass_time_waiting_proc(rr);
        rr_run_proc(rr);
        rr_pass_time(rr);
}

void rr_run_all_before_preemption(RoundRobin *rr) {
        rr_add_new_proc(rr);
        rr_waiting_to_ready(rr);
        rr_running_to_ready(rr);
        rr_ready_to_running(rr);
}

void rr_run(RoundRobin *rr) {
        rr_run_all_after_preemption(rr);
        rr_run_all_before_preemption(rr);
}
