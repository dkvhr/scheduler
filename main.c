#include <stdio.h>
#include <stdlib.h>
#include"types.h"

ProcessList* create_proc_list(){
    ProcessList* pl = (ProcessList*) malloc(sizeof(ProcessList));
    pl->size  = 2;
    pl->procs = (Process**) malloc(pl->size*sizeof(Process*));

    ProcessIO** array1IO = (ProcessIO**) malloc(sizeof(ProcessIO*));
    ProcessIO* io1 = (ProcessIO*) malloc(sizeof(ProcessIO));
    io1->activation_time = 3;
    io1->type = 2;
    array1IO[0] = io1;

    
    ProcessIO** array2IO = (ProcessIO**) malloc(sizeof(ProcessIO*));
    ProcessIO* io2 = (ProcessIO*) malloc(sizeof(ProcessIO));
    io2->activation_time = 2;
    io2->type = 0;
    array2IO[0] = io2;


    IORequest* placeholder1 = (IORequest*)malloc(sizeof(IORequest*));
    placeholder1->request = array1IO;
    placeholder1->size = 1;

    IORequest* placeholder2 = (IORequest*)malloc(sizeof(IORequest*));
    placeholder2->request = array2IO;
    placeholder2->size = 1;


    Process* p1 = (Process*) malloc(sizeof(Process));
    p1->pid = 101;
    p1->status = 0;
    p1->duration = 10;
    p1->arrival_time = 5;    
    p1->IO_req = placeholder1;
    p1->remaining_time = p1->duration;

    Process* p2 = (Process*) malloc(sizeof(Process));
    p2->pid = 102;
    p2->status = 0;
    p2->duration = 12;
    p2->arrival_time = 2;    
    p2->IO_req = placeholder2;
    p2->remaining_time = p2->duration;

    pl->procs[0] = p1;
    pl->procs[1] = p2;

    return pl;
}


int main(void) {
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
                rr.time_elapsed++;
        }
        return 0;
} 
