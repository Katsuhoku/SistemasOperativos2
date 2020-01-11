#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>
#include "Proceso.h"

int main (int argc, char *argv[]){
  key_t key;
  int id_mem;
  proc* mem = NULL;
  int i, j;

  key = ftok(MEM_FILE, MEM_HAND);
  if (key == -1) {
    printf("Cannot get key");
    return(0);
  }

  id_mem = shmget(key, sizeof(proc)*3, 0777 | IPC_CREAT);
  if (id_mem == -1) {
    printf("Cannot get ID");
    return(0);
  }

  mem = (proc *) shmat(id_mem, (char*) 0, 0);
  if (mem == NULL) {
    printf("Cannot get memory");
    return(0);
  }

  // mem es ahora un arreglo de tipo "proc", compartido con cualquier
  // proceso que acceda a él
  for (i = 0; i < 3; i++) {
    mem[i].pid = i;
    mem[i].priority = i + 1;
    mem[i].cputime = (i+1) * 10;
    mem[i].waittime = 0;
    mem[i].totaltime = 0;
  }

  if (id_mem != -1) {
    shmdt((char *) mem);
  }

  return 0;
}
