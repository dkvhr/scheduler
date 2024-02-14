#ifndef TYPES_H
#define TYPES_H

typedef struct PROCESS_ {
        char pid[3];
        char ppid[3];
        int priority;
        int status;
} PROCESS;

#endif
