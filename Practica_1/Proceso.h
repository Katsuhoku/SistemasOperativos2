#ifndef _COMMON_H_
#define _COMMON_H_

#define MEM_FILE "/bin/ls"
#define MEM_HAND 8

// Estructura de información de un proceso
typedef struct procc{
    int pid;
    int priority;
    int cputime;
    int waittime;
    int totaltime;
} proc;

// Lista ligada de procesos
typedef struct node {
    proc process;
    struct node* next;
} procnode;

#endif
