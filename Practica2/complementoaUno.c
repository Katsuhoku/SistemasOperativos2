#include <stdio.h>
#include <stdlib.h>

//definiciones
#define MASCARAXOR 0xFFFFFFFF

//Prototipos
void printInt(int Integer);
void printByte(char byte);
/*
 *  Main
 */
int main( int argc, char **argv ){
    int numero = 0, complemento = 0;

    printf("\tPrograma que optiene el complemento a 1\n");
    printf("Ingrese un numero\n->");
    scanf("%d", &numero);//Bug? se queda en negro
    printf("\nNumero en Bytes:\n");

    complemento = numero ^ MASCARAXOR;//Complemento a 1
    imprimirEntero(complemento);
    return 0;
}

void printByte(char byte){
    for (int i = 0; i < 8; i++) {
        printf("%d", (byte & 0x80) >> 7);
        byte <<= 1;
    }
}

void printInt(int Integer){ 
    int mascara = 0xFF000000;
    for(int i = 0; i < 4; i++){
        imprimirByte((Integer & mascara) >> 24);
        printf(" ");
        Integer <<= 8;
    }
}