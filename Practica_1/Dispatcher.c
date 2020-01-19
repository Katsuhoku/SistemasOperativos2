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

// Constantes
#define QUANTUM 5

// Prototipos
int tickets(procnode* head);
procnode* lottery(procnode* head);
procnode* node(proc newproc);
void add(procnode** head, procnode* new);
void finish(procnode** head, procnode* comp);
void updtwait(procnode* head, int execPID);


int main() {
  int systime = 0; // Tiempo total de ejecución del despachador
  procnode* head = NULL; // Lista de procesos listos
  procnode* finished = NULL; // Lista de procesos finalizados
  procnode* exec = NULL; // Proceso en ejecución
  proc* mem = NULL; // Apuntador a memoria compartida
  int Q = 1;
  int i, cantproc = 0, id_mem;

  time_t t;

  srand((unsigned) time(&t));

  // Genera / Obtiene la memoria compartida
  switch ((id_mem = getshmem(&mem))) {
    case -1:
      fprintf(stderr, "\nError: Cannot get key");
      exit(0);
      break;
    case -2:
      fprintf(stderr, "\nCannot get ID");
      exit(0);
      break;
    case -3:
      fprintf(stderr, "\nCannot get shared memory");
      exit(0);
      break;
  }

  printf("PID\tWait Time\tTotal Time\n");

  while(mem[0].pid != -2 || cantproc > 0) {
    // Obtención de procesos entrantes por el cargador
    i = 0;
    while (i < MAXPROCESS && mem[i].pid > 0) {
      add(&head, node(mem[i])); // Genera nodo y lo agrega
      mem[i].pid = -1;
      i++;
      cantproc++;
    }

    usleep(1000000);

    // Simulación de ejecución
    if(exec != NULL) { // Hay un proceso en ejecución
      if((exec->process).cputime == 0) { // Proceso finalizado
        finish(&head, exec);
        printf("%d\t%d\t\t%d\n", exec->process.pid, exec->process.waittime, exec->process.totaltime);
        add(&finished, exec);
        cantproc--;
        exec = NULL;

        if(head != NULL) {
          exec = lottery(head);
          Q = 0;
        }
      }
      else if (Q == QUANTUM) { // Quantum agotado
        exec = lottery(head);
        Q = 0;
      }

      if (cantproc > 0) {
        (exec->process).cputime--;
        Q++;
        updtwait(head, exec->process.pid);
      }
    }
    else { // No hay proceso en ejecución
      if(head != NULL) {
        exec = lottery(head);
        Q = 0;

        (exec->process).cputime--;
        Q++;
        updtwait(head, exec->process.pid);
      }
    }

    systime++;
  }

  printf("\n\nTotal System Time:\t%d\n\n", systime);

  // Libera la memoria compartida y termina
  outmem(id_mem, (char *) mem);
  clsmem(id_mem);
  return 0;
}

// Funciones

// Calcula el total de tickets
// El total de tickets es la suma de todas las prioridades de todos
// los procesos en la lista
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
// El número de tickets de un proceso es igual a su prioridad
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
  procnode* new = NULL;

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
}


// Actualiza los tiempos de espera para todos los procesos menos el que
// se encuentra actualmente en ejecución
void updtwait(procnode* head, int execPID) {
  procnode* temp = head;

  while(temp != NULL) {
    if (temp->process.pid != execPID) temp->process.waittime++;
    temp->process.totaltime++;
    temp = temp->next;
  }
}
