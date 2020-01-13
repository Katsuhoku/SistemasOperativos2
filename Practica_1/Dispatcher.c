/*
  Benemérita Universidad Autónoma de Puebla
  Facultad de Ciencias de la Computación

  Sistemas Operativos II

  Práctica 1: Despachador Lotería-Por Prioridades (Simulador)

  Arizmendi Ramírez Esiel Kevin
  Coria Rios Marco Antonio
  Ruiz Lozano Paulo César
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "common.h"
#define QUANTUM 5

int tickets(procnode* head);
procnode* lottery(procnode* head);
procnode* node(proc newproc);
void add(procnode** head, procnode* new);
void finish(procnode** head, procnode* comp);
void updtwait(procnode* head, int execPID);

int main() {
  int systime; // Tiempo total de ejecución del despachador
  procnode* head = NULL; // Lista de procesos listos
  procnode* finished = NULL; // Lista de procesos finalizados
  procnode* exec = NULL; // Proceso en ejecución
  proc* mem = NULL; // Apuntador a memoria compartida
  int Q = 1;
  int i;

  time_t t;

  srand((unsigned) time(&t));

  // Genera / Obtiene la memoria compartida
  switch (getshmem(&mem)) {
    case 1:
      printf("\nError: Cannot get key");
      exit(0);
      break;
    case 2:
      printf("\nCannot get ID");
      exit(0);
      break;
    case 3:
      printf("\nCannot get shared memory");
      exit(0);
      break;
    default:
      printf("\nShared memory correctly generated/obtained");
  }

  while(1) {
    // Obtención de procesos entrantes por el cargador
    i = 0;
    printf("\nPID: %d", mem[i].pid);
    while (i < MAXPROCESS && mem[i].pid > 0) {
      add(&head, node(mem[i])); // Genera nodo y lo agrega
      mem[i].pid = -1;
      i++;
    }

    // Simulación de ejecución
    if(exec != NULL) {
      if((exec->process).cputime == 0) { // Proceso finalizado
        printf("\nCompleted");
        finish(&head, exec);
        add(&finished, exec);
        exec = NULL;

        if(head != NULL) {
          exec = lottery(head);
          Q = 0;
          printf("\nWinner: Process PID #%d", exec->process.pid);
        }
      }
      else if (Q == QUANTUM) { // Quantum agotado
        printf("\nTime Out");
        exec = lottery(head);
        Q = 0;
        printf("\nWinner: Process PID #%d", exec->process.pid);
      }

      (exec->process).cputime--;
      Q++;
      systime++;
      updtwait(head, exec->process.pid);
      printf("\nPID #%d\tCPU Time: %d\tWait Time: %d", (exec->process).pid, (exec->process).cputime, (exec->process).waittime);
      printf("\nQ: %d, System Time: %d", Q, systime);
    }
    else {
      if(head != NULL) {
        exec = lottery(head);
        Q = 0;
        printf("\nWinner: Process PID #%d", exec->process.pid);

        (exec->process).cputime--;
        Q++;
        systime++;
        updtwait(head, exec->process.pid);
      }
    }

    sleep(1);
  }

  /*
    Escribir en el archivo la información en la lista de finalizados
  */
  return 0;
}

// Calcula el total de tickets
int tickets(procnode* head) {
  int sum = 0;
  procnode* temp = head;

  while(temp != NULL) {
    sum += (temp->process).priority;
    temp = temp->next;
  }

  return sum;
}

// Selección del proceso a ejecutar por sorteo (lotería)
procnode* lottery(procnode* head) {
  int T = tickets(head);
  int win;
  procnode* temp = head;

  win = (rand() % T) + 1;

  while(temp != NULL) {
    win -= (temp->process).priority;
    if(win <= 0) return temp;
    else temp = temp->next;
  }
}

// Crea un nodo de proceso y lo retorna
procnode* node(proc newproc) {
  procnode* new;

  if ((new = malloc(sizeof(procnode))) != NULL) {
    new->process.pid = newproc.pid;
    new->process.priority = newproc.priority;
    new->process.cputime = newproc.cputime;
    new->process.waittime = newproc.waittime;
    new->process.totaltime = newproc.totaltime;
    new->next = NULL;
  }

  return new;
}

// Agrega un proceso (o una lista de procesos) a la lista
void add(procnode** head, procnode* new) {
  procnode* temp = *head;

  if (*head == NULL) {
    *head = new;
    return;
  }

  while(temp->next != NULL) temp = temp->next;
  temp->next = new;
}

// Elimina un proceso ya completado de la lista de listos
void finish(procnode** head, procnode* comp) {
  procnode* temp = *head;

  if(comp == *head) {
    *head = (*head)->next;
  }
  else {
    while(temp->next != comp) temp = temp->next;
    temp->next = comp->next;
  }

  comp->next = NULL;
  comp->process.totaltime = comp->process.cputime + comp->process.waittime;
}

void updtwait(procnode* head, int execPID) {
  procnode* temp = head;

  while(temp != NULL) {
    if (temp->process.pid != execPID) temp->process.waittime++;
    temp = temp->next;
  }
}
