/*
Hacer un programa que crea un proceso hijo tras mostrar un mensaje identificativo,
el padre esperará hasta que termine el proceso hijo.
EL proceso hijo se comunicará con el padre mediante tuberías y sincronización con señales. EN la primera comunicación
con el hijo le enviará su identificador de proceso y utilizará la señal SIGUSR1 como notificación. A continuación
realizará un bucle con 5 de iteraciones y en cada iteración enviará al padre el valor del índice y utilizará SIGUSR2
como notificación.
DEspués el hijo finalizará.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>

int tuberia[2];

void p()
{
    printf("     Se ha producido la señal SIGUSR1\n");
    int pid_hijo_p;
    read(tuberia[0], &pid_hijo_p, sizeof(pid_hijo_p));
    printf("     (Función p) Padre. PID Hijo: %d \n", pid_hijo_p);
    printf("\n");
}

void q()
{
    printf("     Se ha producido la señal SIGUSR2\n");
    int indice;
    for (int i = 0; i < 5; i++)
    {
        read(tuberia[0], &indice, sizeof(indice));
        printf("     (Función q) Padre. Mensaje: %d \n", indice);
    }
    printf("\n");
}

int main(int argc, char const *argv[])
{
    int pid_fin, estado;
    int pid_hijo1;
    pipe(tuberia);

    signal(SIGUSR1, p);
    signal(SIGUSR2, q);

    printf("(PADRE) Soy el padre con PID %d\n", getpid());

    pid_hijo1 = fork();
    if (pid_hijo1 == 0)
    {
        // Hijo
        printf("(HIJO) Tengo PID %d y mi padre tiene el PID %d\n", getpid(), getppid());
        printf("\n");

        int pid_hijo = getpid();
        write(tuberia[1], &pid_hijo, sizeof(pid_hijo));
        kill(getppid(), SIGUSR1);

        sleep(0.5);

        for (int i = 0; i < 5; i++)
        {
            write(tuberia[1], &i, sizeof(i));
        }

        kill(getppid(), SIGUSR2);

        exit(0);
    }
    else
    {

        pid_fin = wait(&estado); // Hasta que el proceso hijo no termine, el proceso padre no finaliza.
        // Padre
        printf("(PADRE) Soy el padre con PID %d y he creado a mi hijo con PID %d\n", getpid(), pid_hijo1);
    }
    return 0;
}
