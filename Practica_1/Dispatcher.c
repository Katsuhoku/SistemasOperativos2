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
#include <unistd.h>
#define QUANTUM 5

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

int tickets(procnode* head);
procnode* lottery(procnode* head);
void add(procnode* head, procnode* new);
void finish(procnode** head, procnode* comp);

int main (int argc, char *argv[]) {
    int systime; // Tiempo total de ejecución del despachador
    int pickSleep; // Tiempo de espera para buscar nuevos procesos en el archivo
    procnode* head = NULL; // Lista de procesos listos
    procnode* finished = NULL; // Lista de procesos finalizados
    procnode* exec = NULL; // Proceso en ejecución
    int Q = 1;

    time_t t;

    srand((unsigned) time(&t));

    while(1) {
        if(pickSleep == 0) {
            /*
            Ingreso de procesos por lotes
            Apertura del archivo y carga de procesos en lista ligada
            */
        }
        if(exec != NULL) {
            if(cputime == 0) {
                finish(head, exec);
                add(finished, exec);

                if(head != NULL) {
                    exec = lottery(head);

                    (exec->process).cputime--;
                    Q++;
                    systime++;
                }
            }
            else if (Q == QUANTUM) {
                exec = lottery(head);

                (exec->process).cputime--;
                Q++;
                systime++;
            }
        } else {
            if(head != NULL) {
                exec = lottery(head);

                (exec->process).cputime--;
                Q++;
                systime++;
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
