#ifndef TYPES_H
#define TYPES_H

// Declaracao de struct
typedef struct Process Process;
typedef struct ProcessIO ProcessIO;

#define MAX_PROCS 1024
#define MAX_IO_PROCS 1024

// Tempos de E/S
#define DISK_TIME 2
#define TAPE_TIME 4 
#define PRINTER_TIME 6

// fila de um processo normal
typedef struct ProcessNode {
        Process *proc;
        struct ProcessNode *next_node;
        int queue_time;
} ProcessNode;

typedef struct IORequest {
        ProcessIO **request;
        int size;
} IORequest;

typedef struct NodeHead {
        int full_size;
        ProcessNode *front;
        ProcessNode *rear;
        int priority;
} NodeHead;

typedef struct ProcessList {
        Process **procs;
        int size;
} ProcessList;

typedef struct Process{
        int pid;
        int ppid;
        int priority;  // 0=alta, 1=baixa
        int status;  //0=Pronto, 1=Rodando, 2=Bloqueado, 3=Terminado 
        int arrival_time; // quando o proc chega
        int duration;  //tempo de duracao do proc
        int total_exec;  // tempo total de execucao ate o momento
        int activation_time;  //instante de ativacao do proc
        int remaining_time;  // tempo restante ate o proc finalizar
        int end_time;  // instante de finalizacao do proc
        int num_of_IOs;  // numero de IOs que o processo chama
        IORequest *IO_req;  // IOs que o proc chama
        int IO_return_time;  //instante que processo retornará do IO que está sendo executado no momento
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

typedef struct RoundRobin {
        int quantum;
        int time_elapsed;
        int max_procs;
        Process *executing_proc;
        NodeHead *high_priority;
        NodeHead *low_priority;
        NodeHead **IO_queue;

} RoundRobin;

// Function declarations
Process *create_process(int pid, int ppid, int duration, int activation_time, int num_of_IOs, NodeHead *processIOs);  
NodeHead *create_node_head();
ProcessNode *create_process_node(Process *proc);
ProcessIO *create_IO_proc(int type, int activation_time);
int node_head_enqueue(NodeHead *queue, Process *proc);
Process *node_head_dequeue(NodeHead *queue);

#endif
