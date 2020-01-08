#include <stdio.h>
#include <unistd.h>
#define ARGUMENTOS "c:h"

int main (int argc, char *argv[]) {
	int opcion = 0;
	char *opcion1Valores = NULL;

	while ( (opcion = getopt(argc, argv, ARGUMENTOS)) != -1) {
		switch( opcion ) {
			case 'c':
				/*
				 optarg tiene los valores despues del argumento
				 -algo valores
				 optarg = valores;
				 si no existen valores
				 optarg = (NULL);
				 */
				opcion1Valores = optarg;
				printf("\nOpcion i = %s", opcion1Valores);
			break;
			case 'h':
				printf("");
				printf("Parametos admitidos\n -c valor \n");
				printf("-o\n -h ayuda\n");
				return 0;
			break;
			case '?':
				/*
				 getopt retorna ? para parametros desconocidos
				 */
				if (optopt == 'i') {
					printf("\nMissing mandatory input option");
				} else {
					printf("\nParametro desconocido");
				}
			break;
		}
	}

	printf("\n");
	return 0;
 }