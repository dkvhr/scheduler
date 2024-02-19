#include <stdio.h>
#include <stdlib.h>
#include"types.h"

void start_screen();
ProcessList *create_proc_list();
void print_info(RoundRobin *rr);

int main(void) {
        start_screen();
        RoundRobin rr = round_robin_init();
        FILE *file = fopen("entrada.txt", "r");
        if(file == NULL) {
                fprintf(stderr, "Nao foi possivel abrir o arquivo");
                exit(EXIT_FAILURE);
        }
        int n_procs;
        IORequest *io_req;
        ProcessIO **procs_io;
        fscanf(file, "%d", &n_procs);
        for(int i=0; i<n_procs; i++) {
                int pid, ppid, duration, arrival_time;
                io_req = create_IO_request(NULL, 0);
                Process *proc = create_process(pid, ppid, duration, arrival_time, io_req);
                node_head_enqueue(rr.new_procs, proc);
                fscanf(file, "%d %d %d %d %d", &proc->pid, &proc->ppid, &proc->duration, &proc->arrival_time, &io_req->size);
                printf("\nProcesso [%d]\n", proc->pid);
                printf("Duracao de %d. Tempo de chegada: %d\n", proc->duration, proc->arrival_time);

                if(io_req->size > 0) {
                        procs_io = create_IO_proc_ptr_ptr(io_req->size);
                        for(int i=0; i<io_req->size; i++) {
                                int activation_time, type;
                                fscanf(file, "%d %d", &type, &activation_time);
                                ProcessIO *proc_io = create_IO_proc(type, activation_time);
                                node_IO_head_enqueue(rr.IO_proc_queue, proc_io);
                                procs_io[i] = proc_io;
                        }
                        io_req->request = procs_io;
                }
                else if(io_req->size == 0)
                        io_req->request = NULL;

                proc->IO_req = io_req;
                proc->remaining_time = proc->duration;
        }


        while(rr_has_active_processes(&rr)) {
                rr_run(&rr);
        }

        return 0;
} 

void start_screen() {
        printf("\n\nRound Robin com feedback\n");
        printf("Autores: Abid Lohan, Davi Gomes, João Schmidt\n");
        printf("\n\nQuantum escolhido: %d\n", QUANTUM);
        printf("Numero maximo de procs: %d\n", MAX_PROCS);
}
