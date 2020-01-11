/*
 *   Benemérita Universidad Autónoma de Puebla
 *   Facultad de Ciencias de la Computación
 *
 *   Sistemas Operativos II
 *
 *   Práctica 1: Despachador Lotería-Por Prioridades (Simulador)
 *
 *   Arizmendi Ramírez Esiel Kevin
 *   Coria Rios Marco Antonio
 *   Ruiz Lozano Paulo César
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/shm.h>
#include <unistd.h>
#include "Proceso.h"
#define QUANTUM 5

int tickets(procnode* head);
procnode* lottery(procnode* head);
void add(procnode* head, procnode* new);
void finish(procnode** head, procnode* comp);

int main (int argc, char *argv[]) {
  key_t key;
  int id_mem;
  proc* mem = NULL;
  int i, j;

  key = ftok(MEM_FILE, MEM_HAND);
  if (key == -1) {
    printf("Cannot get key");
    exit(EXIT_FAILURE);
  }

  id_mem = shmget(key, sizeof(proc)*3, 0777 | IPC_CREAT);
  if (id_mem == -1) {
    printf("Cannot get ID");
    return(0);
  }

  mem = (proc*) shmat(id_mem, (char*) 0, 0);
  if (mem == NULL) {
    printf("Cannot get memory");
    return(0);
  }

  sleep(5);

  // mem es ahora un arreglo de tipo "proc", compartido con cualquier
  // proceso que acceda a él
  for (i = 0; i < 3; i++) {
    printf("\nPID :\t%d", mem[i].pid);
    printf("\nPRIORITY :\t%d", mem[i].priority);
    printf("\nCPUTIME :\t%d", mem[i].cputime);
    printf("\nWAITTIME :\t%d", mem[i].waittime);
    printf("\nTOTALTIME :\t%d\n", mem[i].totaltime);
  }

  shmdt((char *) mem);
  shmctl(id_mem, IPC_RMID, (struct shmid_ds *)NULL);

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
        if(win <= 0)
            return temp;
        else
            temp = temp->next;
    }
}

// Agrega un proceso (o una lista de procesos) a la lista
void add(procnode* head, procnode* new) {
    procnode* temp = head;

    while(temp->next != NULL)
        temp = temp->next;
    temp->next = new;
}

// Elimina un proceso ya completado de la lista de listos
void finish(procnode** head, procnode* comp) {
    procnode* temp = *head;

    if(comp == *head)
        *head = (*head)->next;
    else 
        while(temp->next != comp) temp = temp->next;
            temp->next = comp->next;


    comp->next = NULL;
}
