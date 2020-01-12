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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/shm.h>
#include "Proceso.h"

//
// Constantes
//
#define MAXLENGTH 50 //Longitud maxima de las cadenas

#define MINPROCESS 1 //Minimo de procesos a enviar
#define MAXPROCESS 15 //Maximo de procesos a enviar

#define MINTIME 1 //Minimo de tiempo dormido
#define MAXTIME 5 //Maximo de tiempo dormido

#define FILENAME "ProcesosGenerados.txt"

#define QUANTUM 5 //QUANTUM

//
//Prototipos
//
void listar(procnode *head);

//
//Main
//
int main (int argc, char *argv[])  {
    procnode* head = NULL; //Lista de procesos
    procnode* last = NULL; //Apuntador al ultimo proceso de la lista de procesos
    
    time_t t;
    key_t key;
    int id_mem;
    proc* mem = NULL;
    
    if ((key = ftok(MEM_FILE, MEM_HAND)) == -1) {
      printf("Cannot get key");
      exit(EXIT_FAILURE);
    }

    
    if ((id_mem = shmget(key, 
                        sizeof(proc)*15, 
                        0777 | IPC_CREAT))
                == -1) {
      printf("Cannot get ID");
      exit(EXIT_FAILURE);
    }

    
    if ((mem = (proc *) shmat(id_mem, (char*) 0, 0)) == NULL) {
      printf("Cannot get memory");
      exit(EXIT_FAILURE);
    }
    
    srand((unsigned) time(&t));
    
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

/*
 * Leer los procesos del archivo y los añade a una lista
 */
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

/* 
 * Envia de MINPROCESS hasta MAXPROCESS al dispatcher
 */
void sendProcesses(procnode** head, proc** mem){
    if ((*head) == NULL) return;
    int cantProcesses = rand() % (MAXPROCESS - MINPROCESS + 1) + MINPROCESS;
    int j = cantProcesses;
    printf("Valor de j: %d \n", j);
    
    for (int i = 0; j > 0 && (*head) != NULL ; i++, j--){
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
  if (head == NULL) {
    printf("\nLista Vacía\n");
    return;
  }

  printf("\nDatos Almacenados:\n");
  for (procnode* temp = head; temp != NULL; temp = temp->next)
    printf("PID: %d EXE: %d, Priority: %d\n", temp->process.pid, temp->process.cputime, temp->process.priority);
}
