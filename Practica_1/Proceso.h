#ifndef _PROCESO_H_
#define _PROCESO_H_

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



// Agrega un proceso (o una lista de procesos) a la lista
void add(procnode** head, procnode** last, proc process){
    procnode *new;
    
    if ((new = malloc(sizeof(procnode))) == NULL) {
        return;
    }
    new->process = process;
    new->next = NULL;
    
    if ((*head) == NULL) {
        (*head) = (*last) = new;
    }else{
        (*last)->next = new;
        (*last) = new;
    }
}

/*
 * Borra un nodo de la lista ligada
 * Recibe:
 * Cabeza de la lista ligada
 */
void delete(procnode** head){
    if ((*head) != NULL) {
        procnode* temp = (*head);
        (*head) = (*head)->next;
        free(temp);
    }
}

void sendProcesses(procnode** head, proc** mem);
void readProcesses(procnode** head, procnode** last);

#endif
