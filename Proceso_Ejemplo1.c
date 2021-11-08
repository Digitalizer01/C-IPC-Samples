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

int funcion1(int a);

int main(int argc, char const *argv[])
{
    // Proceso principal
    printf("(PADRE) Soy el proceso %d y voy a crear un proceso nuevo.\n", getpid());

    int pid = fork();

    if (pid == 0)
    {
        // Hijo
        printf("(HIJO) Soy el proceso %d, mi padre es %d \n", getpid(), getppid());
        printf("(HIJO) Soy el proceso %d y voy a finalizar.\n", getpid());

        exit(0);
    }
    else
    {
        // Padre
        printf("(PADRE) Soy %d, el proceso %d creado con éxito .\n", getpid(), pid);
        sleep(1); // Lo colocamos para evitar que el padre acabe antes que el hijo. Si acabara antes que el hijo, el hijo mostraría que
                  // su padre es el abuelo.
        printf("(PADRE) Soy %d y voy a finalizar.\n", getpid());

        exit(0);
    }
    return 0;
}