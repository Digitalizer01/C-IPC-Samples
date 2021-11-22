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
int tuberia_2[2];

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
    char mensaje[1000];
    for (int i = 0; i < 5; i++)
    {
        read(tuberia_2[0], mensaje, sizeof(mensaje));
        printf("     (Función q) Padre. Mensaje: %s \n", mensaje);
    }
    printf("\n");
}

int main(int argc, char const *argv[])
{
    int pid_fin, estado;
    int pid_hijo1;
    pipe(tuberia);
    pipe(tuberia_2);

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

        char mensaje[1000];

        for (int i = 0; i < 5; i++)
        {
            sprintf(mensaje, "%d", i);
            write(tuberia_2[1], mensaje, sizeof(mensaje));
        }

        kill(getppid(), SIGUSR2);

        exit(0);
    }
    else
    {
        signal(SIGUSR1, p);
        signal(SIGUSR2, q);

        pid_fin = wait(&estado); // Hasta que el proceso hijo no termine, el proceso padre no finaliza.
        // Padre
        printf("(PADRE) Soy el padre con PID %d y he creado a mi hijo con PID %d\n", getpid(), pid_hijo1);
    }
    return 0;
}
