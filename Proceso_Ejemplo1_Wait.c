/*
Realizar un proceso en C, que cree un proceso hijo/nuevo. El proceso principal antes de crear un nuevo proceso muestra
"Soy el proceso XX y voy a crear un proceso nuevo" y crea el proceso. EL proceso hijo muestra el mensaje "Soy el proceso YY,
mi padre es XX", a continuación muestra "Soy el proceso YY y voy a finalizar" y a continuación finaliza.
El proceso padre una vez creado el nuevo proceso muestra el mensaje "Proceso YY creado con éxito" y finaliza.
*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int funcion1(int a);

int main(int argc, char const *argv[])
{
    // Proceso principal
    printf("(PADRE) Soy el proceso %d y voy a crear un proceso nuevo.\n", getpid());
    int pidfin, estado;
    int e1, e2;

    int pid = fork();

    if (pid == 0)
    {
        // Hijo
        printf("(HIJO) Soy el proceso %d, mi padre es %d \n", getpid(), getppid());
        printf("(HIJO) Soy el proceso %d y voy a finalizar.\n", getpid());

        exit(9);
    }
    else
    {
        // Padre
        printf("(PADRE) Soy %d, el proceso %d creado con éxito .\n", getpid(), pid);

        pidfin = wait(&estado); // Hasta que el proceso hijo no termine, el proceso padre no finaliza.

        // --------------------- INICIO TAREA DEL PADRE ---------------------
        e1 = estado >> 8;
        e2 = estado & 0x00ff;

        printf("Valor de e1: %d \n", e1);
        printf("Valor de e2: %d \n", e2);

        if (e2 == 0)
        {
            printf("Proceso HIJO con identificador de proceso %d finaliza por exit(%d) \n", pidfin, e1);
        }
        else
        {
            printf("Proceso HIJO con identificador de proceso %d finaliza por una señal. \n", pidfin);
        }

        // ¡¡¡¡¡¡¡¡ATENCIÓN!!!!!!!!
        // Hay que incluir tantos wait como hijos haya.

        // ----------------------- FIN TAREA DEL PADRE ----------------------

        printf("(PADRE) Soy %d y voy a finalizar.\n", getpid());

        exit(0);
    }
    return 0;
}