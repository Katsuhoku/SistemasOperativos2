#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
//Constantes
#define ARGUMENTOS "c:h"

#define MaxPid 97128765
#define MinPid 1000000

#define MaxTimeEjecu 200
#define MinTimeEjecu 5

#define MaxPriority 5
#define MinPriority 1


int main (int argc, char *argv[]) {
	int opcion, cantidadProcesos = 0, pid, tiempoEspera, prioridad;
	FILE *fichero;

	while ( (opcion = getopt(argc, argv, ARGUMENTOS)) != -1) {
		switch( opcion ) {
			case 'c'://Cantidad procesos
				cantidadProcesos = atoi(optarg);
			break;

			case 'h'://Ayuda
				printf("Programa generador de procesos en un Archivo\n");
				printf("Uso:\n-c <Int:cantidad>  [Indica al programa cuandos procesos se crearan] \n");
				printf("-h [ayuda]\n");
				printf("\n Ejemplo: ./Generador -c 200");
				exit(EXIT_SUCCESS);
			break;

			case '?'://Argumento no reconocido o falta de argumentos
				if (optopt == 'c') {
					printf("\nFanta indicar cantidad de procesos\n");
				} else {
					printf("\nArgumento desconocido\n");
				}
				printf("Use el argumento -h para tener informacion\n");
				exit(EXIT_FAILURE);
			break;
		}
	}
	/*
	 * Inicio de proceso
	 */
	if (cantidadProcesos ==0 ){
		printf("Error en la lectura del parametro -c\n");
		exit(EXIT_FAILURE);
	}
	
	//Apertura de Archivo
	if ((fichero = fopen("ProcesosGenerados.txt", "w")) == NULL){
		printf("Error al abrir el archivo\n");
		exit(EXIT_FAILURE);
	}
	srand ( time(NULL) + 52 );

	for (int i = 0; i < cantidadProcesos; i++){
		//pid        = rand () % (MaxPid - MinPid + 1) + MinPid;
		tiempoEspera = rand () % (MaxTimeEjecu - MinTimeEjecu + 1) + MinTimeEjecu;
		prioridad    = rand () % (MaxPriority - MinPriority + 1) + MinPriority;

		fprintf(fichero, "%d", i+1);
		fprintf(fichero, " ");
		fprintf(fichero, "%d", tiempoEspera);
		fprintf(fichero, " ");
		fprintf(fichero, "%d", prioridad);
		fprintf(fichero, "\n");
	}
	
	if (fclose( fichero ) != 0){
    	printf("Error al cerrar el fichero\n");
		exit(EXIT_FAILURE);
  	}
	printf("Cantidad de datos introducidos %d\n",cantidadProcesos);
	printf("\n");
	return 0;
 }