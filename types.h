#ifndef TYPES_H
#define TYPES_H

// Declaracao de struct
typedef struct Process Process;
typedef struct ProcessIO ProcessIO;

#define MAX_PROCS 1024
#define MAX_IO_PROCS 1024
#define QUANTUM 4

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

typedef struct ProcessIONode {
  ProcessIO *procIO;
  struct ProcessIONode *next_node;
} ProcessIONode;

typedef struct IORequest {
  ProcessIO **request;
  int size;
} IORequest;

typedef struct NodeHead {
  int full_size;
  ProcessNode *front;
  ProcessNode *rear;
  int priority;
  int size;
} NodeHead;

typedef struct NodeIOHead {
  int full_size;
  int size;
  ProcessIONode *front;
  ProcessIONode *rear;
  int priority;
} NodeIOHead;

typedef struct ProcessList {
  Process **procs;
  int size;
} ProcessList;

typedef struct Process {
  int pid;
  int ppid;
  int priority;             // 0=alta, 1=baixa
  int status;               // 0=Pronto, 1=Rodando, 2=Bloqueado, 3=Terminado
  int arrival_time;    // quando o proc chega
  unsigned duration;        // tempo de duracao do proc
  unsigned total_exec;      // tempo total de execucao ate o momento
  unsigned activation_time; // instante de ativacao do proc
  int remaining_time;  // tempo restante ate o proc finalizar
  unsigned end_time;        // instante de finalizacao do proc
  unsigned time_waiting;
  int io_type;        // tipo de IO que o proc esta esperando
  IORequest *IO_req;  // IOs que o proc chama
  int IO_return_time; // instante que processo retornará do IO que está sendo
                      // executado no momento
} Process;

typedef struct ProcessIO {
  int type; // 0=Disco, 1=Fita, 2=Impressora
  // tempo de disco = 2
  // tempo de fita = 4
  // tempo de impressora = 6
  int activation_time; // instante no tempo de execucao do processo em que o IO
                       // sera chamado (em relacao ao processo, e nao ao tempo
                       // total)
  int duration;
  int priority; // prioridade que o processo retornara quando terminar esse IO
  unsigned remaining_time;
} ProcessIO;

typedef struct RoundRobin {
  int quantum;
  int time_elapsed;
  unsigned max_procs;
  unsigned active_processes;
  unsigned active_io_processes;
  Process *running_procs; // proc rodando atualmente
  ProcessIO *io_running_procs;
  // filas
  NodeHead *new_procs;
  NodeIOHead *new_io_procs;
  NodeHead *blocked_procs;
  NodeHead *finished_procs;
  NodeHead *high_priority;
  NodeHead *low_priority;
  NodeHead **IO_queue;
  NodeIOHead *IO_proc_queue; // fila unica dos dispositivos de IO

} RoundRobin;

// Function declarations
Process *create_process(IORequest *io_req);
NodeHead *create_node_head(int max_size, int priority);
int queue_is_empty(NodeHead *queue);
ProcessNode *create_process_node(Process *proc);
ProcessIO *create_IO_proc(int type, int activation_time);
int node_head_enqueue(NodeHead *queue, Process *proc);
Process *node_head_dequeue(NodeHead *queue);
RoundRobin round_robin_init();
int rr_check_end_of_processes(RoundRobin *rr, ProcessList *proc_list);
void rr_next_action(RoundRobin *rr, ProcessList *proc_list);
IORequest *create_IO_request(ProcessIO **req, int size);
ProcessIO **create_IO_proc_ptr_ptr(int size);
NodeIOHead *create_node_IO_head(int max_size, int priority);
int node_IO_head_enqueue(NodeIOHead *IO_queue, ProcessIO *proc_io);
int IOqueue_is_empty(NodeIOHead *queue);
ProcessIO *IO_node_head_dequeue(NodeIOHead *queue);
int rr_has_active_processes(RoundRobin *rr);
void rr_run(RoundRobin *rr);
void print_queues(RoundRobin *rr);
void rr_run_all_before_preemption(RoundRobin *rr);
int io_queue_is_empty(NodeIOHead *queue);

#endif
