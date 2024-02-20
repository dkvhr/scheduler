#include "types.h"
#include <stdio.h>
#include <stdlib.h>

/* Aqui implementei uma queue usando linked list*/

ProcessNode *create_process_node(Process *proc) {
  ProcessNode *node;
  if ((node = (ProcessNode *)malloc(sizeof(ProcessNode))) == NULL) {
    fprintf(stderr, "Nao foi possivel alocar memoria");
    exit(EXIT_FAILURE);
  }

  node->proc = proc;
  node->next_node = NULL;

  return node;
}

NodeHead *create_node_head(int max_size, int priority) {
  NodeHead *head;
  if ((head = (NodeHead *)malloc(sizeof(NodeHead))) == NULL) {
    fprintf(stderr, "Nao foi possivel alocar memoria");
    exit(EXIT_FAILURE);
  }

  head->front = NULL;
  head->rear = NULL;
  head->full_size = max_size;
  head->priority = priority;
  head->size = 0;

  return head;
}

NodeIOHead *create_node_IO_head(int max_size, int priority) {
  NodeIOHead *IOhead;
  if ((IOhead = (NodeIOHead *)malloc(sizeof(NodeIOHead))) == NULL) {
    fprintf(stderr, "Nao foi possivel alocar memoria");
    exit(EXIT_FAILURE);
  }

  IOhead->front = NULL;
  IOhead->rear = NULL;
  IOhead->full_size = max_size;
  IOhead->priority = priority;
  IOhead->size = 0;

  return IOhead;
}

int queue_is_empty(NodeHead *queue) { return queue->size == 0; }

int IOqueue_is_empty(NodeIOHead *queue) { return queue->size == 0; }

int node_head_enqueue(NodeHead *queue, Process *proc) {
  ProcessNode *tmp;
  if ((tmp = (ProcessNode *)malloc(sizeof(ProcessNode))) == NULL) {
    fprintf(stderr, "Nao foi possivel alocar memoria");
    exit(EXIT_FAILURE);
  }

  tmp->proc = proc;
  tmp->next_node = NULL;
  if (queue_is_empty(queue)) {
    queue->front = tmp;
    queue->rear = tmp;
  }

  else {
    queue->rear->next_node = tmp;
    queue->rear = tmp;
  }
  queue->size++;
  return 0;
}

int node_IO_head_enqueue(NodeIOHead *IO_queue, ProcessIO *proc_io) {
  ProcessIONode *tmp;
  if ((tmp = (ProcessIONode *)malloc(sizeof(ProcessIONode *))) == NULL) {
    fprintf(stderr, "Nao foi possivel alocar memoria");
    exit(EXIT_FAILURE);
  }

  tmp->procIO = proc_io;
  tmp->next_node = NULL;
  if (IOqueue_is_empty(IO_queue)) {
    IO_queue->front = tmp;
    IO_queue->rear = tmp;
  }

  else {
    IO_queue->rear->next_node = tmp;
    IO_queue->rear = tmp;
  }
  IO_queue->size++;
  return 0;
}

Process *node_head_dequeue(NodeHead *queue) {
  // remove o primeiro elemento da fila e retorna como Process
  if (queue_is_empty(queue)) {
    queue->front = queue->rear = NULL;
    return NULL;
  }

  ProcessNode *tmp;
  tmp = queue->front;
  queue->front = tmp->next_node;
  Process *tmpp = tmp->proc;
  free(tmp);
  if (queue->size == 1)
    queue->rear = NULL;
  queue->size--;
  if (queue_is_empty(queue)) {
    queue->front = queue->rear = NULL;
    return tmpp;
  }

  return tmpp;
}

ProcessIO *IO_node_head_dequeue(NodeIOHead *queue) {
  // remove o primeiro elemento da fila e retorna como Process
  if (IOqueue_is_empty(queue)) {
    queue->front = queue->rear = NULL;
    return NULL;
  }

  ProcessIONode *tmp;
  tmp = queue->front;
  queue->front = tmp->next_node;
  ProcessIO *tmpp = tmp->procIO;
  free(tmp);
  if (queue->size == 1)
    queue->rear = NULL;
  queue->size--;
  if (IOqueue_is_empty(queue)) {
    queue->front = queue->rear = NULL;
    return tmpp;
  }

  return tmpp;
}
