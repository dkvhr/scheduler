#ifndef TYPES_H
#define TYPES_H

#define MAX_PROCS 1024
#define MAX_IO_PROCS 1024

typedef struct ProcessIO{
        int type;  // 0=Disco, 1=Fita, 2=Impressora
        int activation_time;  // instante no tempo de execucao do processo em que o IO sera chamado (em relacao ao processo, e nao ao tempo total)
        int duration;
        int priority;  //prioridade que o processo retornara quando terminar esse IO
} ProcessIO;

typedef struct Process{
        int pid;
        int ppid;
        int priority;  // 0=alta, 1=baixa
        int status;  //0=Pronto, 1=Rodando, 2=Bloqueado, 3=Terminado 
        int duration;  //tempo de duracao do proc
        int total_exec;  // tempo total de execucao ate o momento
        int activation_time;  //instante de ativacao do proc
        int end_time;  // instante de finalizacao do proc
        int num_of_IOs;  // numero de IOs que o processo chama
        struct ProcessIO processIOs[MAX_IO_PROCS];  //lista com IOs que o proc chama
        int IO_return;  //instante que processo retornará do IO que está sendo executado no momento
} Process;

typedef struct Nodes {
        Process proc;
        struct Nodes *next_proc;
} Nodes;

typedef struct IOQueue {
        ProcessIO IO_proc;
        struct IOQueue *next_io_proc;
} IOQueue;

// Function declarations
Process *create_process(int id, int duration);  

#endif
