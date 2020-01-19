#include <stdio.h>

//Prototipos
void printByte(char byte);
/*
 *  Main
 */
int main( int argc, char **argv ) {
    char aux[4];
    int numero;

    printf("Ingrese un numero\n->");
    scanf("%d", &numero);

    // Extracción de bytes
    for (int i = 0; i < 4; i++) {
        aux[3 - i] = numero & 0xFF;
        numero >>= 8;
    }

    // Impresión de resultados
    printf("Los bytes son: ");
    for (int i = 0; i < 4; i++) {
        printByte(aux[i]);
        printf(" ");
    }
    printf("\n");
    return 0;
}

// Funcion que imprime los Bytes
void printByte(char byte){
    for (int i = 0; i < 8; i++) {
        printf("%d", (byte & 0x80) >> 7);
        byte <<= 1;
    }
}