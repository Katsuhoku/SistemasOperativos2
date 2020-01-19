#include <stdio.h>
#include <sys/shm.h>
#include "common.h"

// Genera la memoria compartida o la obtiene si ya fue creada
// Coloca el apuntador en la variable del parámetro y retorna un indicador
int getshmem(proc** mem) {
  key_t key;
  int id_mem;
  *mem = NULL;
  int i;

  key = ftok(MEM_FILE, MEM_HAND);
  if (key == -1) return -1;

  id_mem = shmget(key, sizeof(proc)*MAXPROCESS, 0777 | IPC_CREAT);
  if (id_mem == -1) return -2;

  *mem = (proc*) shmat(id_mem, (char*) 0, 0);
  if (*mem == NULL) return -3;

  return id_mem;
}

// Sale de la memoria compartida
void outmem(int id_mem, char* mem) {
  if (id_mem != -1) shmdt(mem);
}

// Libera la memoria compartida
void clsmem(int id_mem) {
  shmctl(id_mem, IPC_RMID, (struct shmid_ds *)NULL);
}
