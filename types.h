#ifndef TYPES_H
#define TYPES_H

// Declaracao de struct
typedef struct Process Process;

#define MAX_PROCS 1024
#define MAX_IO_PROCS 1024

// Tempos de E/S
#define DISK_TIME 2
#define TAPE_TIME 4 
#define PRINTER_TIME 6

// fila de um processo normal
typedef struct ProcessNode {
        Process *proc;
        struct ProcessNode *prev_node;
        struct ProcessNode *next_node;
} ProcessNode;

typedef struct NodeHead {
        int full_size;
        ProcessNode *first_node;
        ProcessNode *last_node;
} NodeHead;

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
        NodeHead processIOs;  //Head com os IOs que o proc chama
        int IO_return;  //instante que processo retornará do IO que está sendo executado no momento
} Process;

typedef struct ProcessIO{
        int type;  // 0=Disco, 1=Fita, 2=Impressora
        // tempo de disco = 2
        // tempo de fita = 4
        // tempo de impressora = 6 
        int activation_time;  // instante no tempo de execucao do processo em que o IO sera chamado (em relacao ao processo, e nao ao tempo total)
        int duration;
        int priority;  //prioridade que o processo retornara quando terminar esse IO
} ProcessIO;

// Function declarations
Process *create_process(int pid, int ppid, int duration, int activation_time, int num_of_IOs, NodeHead *processIOs);  

#endif
