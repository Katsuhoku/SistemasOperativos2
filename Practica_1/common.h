#ifndef _COMMON_H_
#define _COMMON_H_

#define MEM_FILE "/bin/ls"
#define MEM_HAND 8

#define MINPROCESS 1 //Minimo de procesos a enviar
#define MAXPROCESS 15 //Maximo de procesos a enviar

// Estructura de información de un proceso
typedef struct {
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

int getshmem(proc** mem);

#endif
