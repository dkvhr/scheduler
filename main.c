#include <stdio.h>
#include <stdlib.h>
#include"types.h"

void start_screen();
ProcessList *create_proc_list();
void print_info(RoundRobin *rr);

int main(void) {
        start_screen();
        RoundRobin rr = round_robin_init();
        ProcessList *proc_list = create_proc_list();

        while(!rr_check_end_of_processes(&rr, proc_list)) {
                if(rr.executing_proc != NULL) {
                        printf("Tempo passado no processo: %d\n", rr.time_elapsed);
                        printf("Quantum escolhido: %d\n", rr.quantum);
                        printf("PID do processo: %d\n", rr.executing_proc->pid);
                        printf("Prioridade do processo: %d\n", rr.executing_proc->priority);
                }
                rr_next_action(&rr, proc_list);
                print_info(&rr);
                rr.time_elapsed++;
        }
        return 0;
} 


ProcessList* create_proc_list(){
        ProcessList* proc_list;
        proc_list = (ProcessList*) malloc(sizeof(ProcessList));
        if(proc_list == NULL) {
                fprintf(stderr, "nao foi possivel alocar memoria\n");
                exit(EXIT_FAILURE);
        }
        proc_list->size  = MAX_PROCS;
        
        Process **procs_on_proc_list;
        procs_on_proc_list = (Process **) malloc(MAX_PROCS * sizeof(Process **));
        if(procs_on_proc_list == NULL) {
                fprintf(stderr, "Nao foi possivel alocar memoria\n");
                exit(EXIT_FAILURE);
        }

        FILE *file = fopen("entrada.txt", "r");
        if(file == NULL) {
                fprintf(stderr, "Nao foi possivel abrir o arquivo\n");
                exit(EXIT_FAILURE);
        }

        for(int i=0; i<MAX_PROCS; ++i) {
                IORequest *io_req = create_IO_request(NULL, 0);
                int pid, ppid, duration, arrival_time;
                fscanf(file,"%d %d %d %d %d", &pid, &ppid, &duration, &arrival_time, &io_req->size);
                Process *proc = create_process(pid, ppid, duration, arrival_time, io_req);

                if(io_req->size > 0) {
                        ProcessIO **procs_io = create_IO_proc_ptr_ptr(io_req->size);
                        for(int i=0; i<io_req->size; ++i) {
                                int activation_time, type;
                                fscanf(file, "%d %d", &type, &activation_time);
                                ProcessIO *proc_io = create_IO_proc(activation_time, type);
                                procs_io[i] = proc_io;
                        }
                        io_req->request = procs_io;
                }
                else if(io_req->size == 0) {
                        io_req->request = NULL;
                }
                proc->IO_req = io_req; //atualizando o valor;
                proc->remaining_time = proc->duration;
                procs_on_proc_list[i] = proc;
        }
        proc_list->procs = procs_on_proc_list;
        proc_list->size = MAX_PROCS;
        fclose(file);
        return proc_list;
}



void start_screen() {
        printf("\n\nRound Robin com feedback\n");
        printf("Autores: Abid Lohan, Davi Gomes, João Schmidt\n");
        printf("\n\nQuantum escolhido: %d\n", QUANTUM);
        printf("Numero maximo de procs: %d\n", MAX_PROCS);
}

int print_current_process(Process *proc) {
        if(proc == NULL) {
                printf("\nNao ha processos ou a entrada esta invalida!\n");
                return 1;
        }

        printf("Processo atual: [ %d ] com status %d\n", proc->pid, proc->status);
        printf("Tempo em que o processo chegou: %d\n", proc->arrival_time);
        printf("Tempo ja passado no processo: %d\n", proc->total_exec);
        printf("Tempo restante do processo: %d\n", proc->remaining_time);
        printf("O processo esta com prioridade de: %d\n\n", proc->priority);
        return 0;
}

int print_current_process_rr(RoundRobin *rr) {
        if(rr->executing_proc == NULL) {
                printf("Nenhum processo sendo executado\n");
                return 1;
        }

        printf("Processo atual:\n");
        print_current_process(rr->executing_proc);
        return 0;
}

void print_info(RoundRobin *rr) {
        printf("Tempo atual: %d\n", rr->time_elapsed);
        print_current_process_rr(rr);
}
