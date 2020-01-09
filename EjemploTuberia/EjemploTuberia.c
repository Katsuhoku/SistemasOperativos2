#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
 
#define SIZE 512
 
int main( int argc, char **argv ){
    pid_t pid;
    int tuberia[2], readbytes;
    char buffer[SIZE];

    /*
     * el apuntador de una pipe es un arrelgo donde 
     * tuberia[1] es el lado de escritura
     * tuberia[0] es el lado de lectura
     */
    pipe( tuberia );

    if ( ( pid=fork() ) == 0 ){ // hijo leera
        close( tuberia[1] ); /* cerramos el lado de escritura del pipe */

        while( (readbytes=read( tuberia[0], buffer, SIZE )) > 0)
            write( 1, buffer, readbytes );

        close( tuberia[0] );
    }
    else{ // padre escribe
        close( tuberia[0] ); /* cerramos el lado de lectura del pipe */

        strcpy( buffer, "Esto llega a traves de la tuberia\n" );
        write( tuberia[1], buffer, strlen( buffer ) );

        close( tuberia[1] );
    }
    waitpid( pid, NULL, 0 );

    return 0;
}