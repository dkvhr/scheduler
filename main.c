#include "types.h"
#include <stdio.h>
#include <stdlib.h>

void start_screen();
ProcessList *create_proc_list();
void print_info(RoundRobin *rr);

int main(void) {
  start_screen();
  RoundRobin rr = round_robin_init();
  FILE *file = fopen("entrada.txt", "r");
  if (file == NULL) {
    fprintf(stderr, "Nao foi possivel abrir o arquivo");
    exit(EXIT_FAILURE);
  }
  int n_procs;
  fscanf(file, "%d", &n_procs);
  for (int i = 0; i < n_procs; i++) {
    // io_req = create_IO_request(NULL, 0);
    int number_of_ios_requests;
    Process *proc = create_process();
    fscanf(file, "%d %d %d %d %d", &proc->pid, &proc->ppid, &proc->duration,
           &proc->arrival_time, &number_of_ios_requests);
    node_head_enqueue(rr.new_procs, proc);
    printf("\nProcesso [%d]\n", proc->pid);
    printf("Duracao de %d. Tempo de chegada: %d\n", proc->duration,
           proc->arrival_time);

    if (number_of_ios_requests) {
      ProcessIO **pproc_io =
          (ProcessIO **)malloc(sizeof(ProcessIO *) * number_of_ios_requests);
      for (int i = 0; i < number_of_ios_requests; i++) {
        int activation_time, type;
        fscanf(file, "%d %d", &type, &activation_time);
        ProcessIO *proc_io = create_IO_proc(type, activation_time);
        switch (proc_io->type) {
        case 0:
          proc_io->duration = 2;
          break;
        case 1:
          proc_io->duration = 4;
          break;
        case 2:
          proc_io->duration = 6;
          break;
        }
        proc_io->remaining_time = proc_io->duration;
        proc_io->activation_time += proc->activation_time;
        node_IO_head_enqueue(rr.new_io_procs, proc_io);
        pproc_io[i] = proc_io;
      }
      proc->number_of_ios_requests = number_of_ios_requests;
      proc->IO_req = pproc_io;
    }

    proc->remaining_time = proc->duration;
    printf("entrando no clock");
  }
  printf("\n");

  // rr_add_new_proc(&rr);
  // rr_add_new_io_proc(&rr);
  rr_run_all_before_preemption(&rr);

  while (rr_has_active_processes(&rr)) {
    printf("Tempo: %d\n", rr.time_elapsed + 1);
    rr_run(&rr);
    print_queues(&rr);
  }

  return 0;
}

void start_screen() {
  printf("**********");
  printf("\n\nRound Robin com feedback\n");
  printf("Autores: Abid Lohan, Davi Gomes, João Schmidt\n\n");
  printf("**********");
  printf("\n\nQuantum escolhido: %d\n", QUANTUM);
  printf("Numero maximo de procs: %d\n", MAX_PROCS);
}
