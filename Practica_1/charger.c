/*##############################################
#  Benemerita Universidad Autonoma de Puebla   #
#  Facultad de Ciencias de la Computacion      #
#  Ing. en Ciencias de la Computacion          #
#  Sistemas Operativos II                      #
#  Primavera 2020                              #
#  Arizmendi Ramirez Esiel Kevin               #
#  Coria Rios Marco Antonio                    #
#  Ruiz Lozano Paulo Cesar                     #
###############################################*/

#define MAXLENGTH 50 //Longitud maxima de las cadenas

#define MINTIME 3 //Minimo de tiempo dormido
#define MAXTIME 10 //Maximo de tiempo dormido

#define FILENAME "procfile.txt"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/shm.h>
#include "common.h"

/*  Prototipos de funciones para Lista Ligada */
void add(procnode** head, procnode** last, proc process);
void delete(procnode** head);
void readProcesses(procnode** head, procnode** last);
void sendProcesses(procnode** head, proc** mem);
void listar(procnode *head);

int main() {
    procnode* head = NULL; //Lista de procesos
    procnode* last = NULL; //Apuntador al ultimo proceso de la lista de procesos

    time_t t;
    srand((unsigned) time(&t));

    proc* mem = NULL;

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


    /* Se lee el archivo con todos los procesos */
    readProcesses(&head, &last);
    procnode* temp = head;
    listar(head);
    while (head != NULL) {
        sendProcesses(&head, &mem);
        //usleep(1000000);
        usleep((rand() % (MAXTIME - MINTIME + 1) + MINTIME) * 1000000);
    }

    return 0;
}

// Agrega un proceso (o una lista de procesos) a la lista
void add(procnode** head, procnode** last, proc process){
    procnode *new;

    if ((new = malloc(sizeof(procnode))) == NULL) {
        return;
    }
    new->process.pid = process.pid;
    new->process.priority = process.priority;
    new->process.cputime = process.cputime;
    new->process.waittime = process.waittime;
    new->process.totaltime = process.totaltime;
    new->next = NULL;

    if ((*head) == NULL) {
        (*head) = (*last) = new;
    }else{
        (*last)->next = new;
        (*last) = new;
    }
}

void delete(procnode** head){
    if ((*head) != NULL) {
        procnode* temp = (*head);
        (*head) = (*head)->next;
        free(temp);
    }
}

//Leer los procesos del archivo y añadirlos a una lista
void readProcesses(procnode** head, procnode** last){
    FILE* file;
    char line[MAXLENGTH];
    proc process;

    if ((file = fopen(FILENAME, "r")) == NULL ) {
        printf("Error al abrir el archivo");
        exit(EXIT_FAILURE);
    }

    while (fgets(line, MAXLENGTH, file)) {
        process.pid = atoi(strtok(line, " ")); //Obtiene el pid
        process.cputime = atoi(strtok(NULL, " ")); //Obtiene el tiempo de ejecución
        process.priority = atoi(strtok(NULL, "\n")); //Obtiene la prioridad
        process.waittime = process.totaltime = 0; //Se inicializa los tiempos de espera y total
        add(head, last, process);

    }
}

//Envia de MINPROCESS hasta MAXPROCESS al dispatcher
void sendProcesses(procnode** head, proc** mem){
    int i;
    if ((*head) == NULL) return;
    int cantProcesses = rand() % (MAXPROCESS - MINPROCESS + 1) + MINPROCESS;
    int j = cantProcesses;
    printf("Valor de j: %d \n", j);

    for (i = 0; j > 0 && (*head) != NULL ; i++, j--){
        //Agregar los procesos a memoria compartida
        (*mem)[i].pid = (*head)->process.pid;
        (*mem)[i].cputime = (*head)->process.cputime;
        (*mem)[i].priority = (*head)->process.priority;
        (*mem)[i].totaltime = (*head)->process.totaltime;
        (*mem)[i].waittime = (*head)->process.waittime;

        //Eliminar el proceso de la lista de procesos
        delete(head);
    }
    (*mem)[cantProcesses-j].pid = -1;

}


void listar(procnode *head) {
  procnode* temp;
  if (head == NULL) {
    printf("\nLista Vacía\n");
    return;
  }

  printf("\nDatos Almacenados:\n");
  for (temp = head; temp != NULL; temp = temp->next)
    printf("PID: %d EXE: %d, Priority: %d\n", temp->process.pid, temp->process.cputime, temp->process.priority);
}
