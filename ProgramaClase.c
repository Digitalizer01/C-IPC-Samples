/*
Programa que crea dos procesos hijos y se comunica con ellos mediante memoria compartida, tuberías y notifación mediante señales.
EL proceso padre cuanto todos los procesos se han creao y han finalizado sus tareas, les enviará una comunicación mediante tubería
y señal para que finalicen. LOs procesos hijos no podrán finalizar hasta recibir esta señal y activar su condición de salida que
encontrará en memoria compartida. En memoria compartida los procesos hijos guardarán su identificador de proceso nada más
crearse y la condición de salida se activará a 0, al recibir la comunicación de finalización pondrán a 1 ese valor y así podrán
finalizar su ejecución limpiamente.
*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>

void leersalida();

int tuberia[2];

typedef struct
{
    int pid;
    int salida;
} datos;

datos *puntero;

int main(int argc, char const *argv[])
{

    int estado;
    int id;
    int pid;
    key_t clave;
    clave = ftok("/bin/ls", 76);
    id = shmget(clave, 2 * sizeof(datos), 0777 | IPC_CREAT); // El dos indica el número de campos del struct
    puntero = (datos *)shmat(id, (char *)0, 0);

    for (int i = 0; i < 2; i++)
    {
        puntero[i].pid = 0;
        puntero[i].salida = 0;
        printf("puntero[%d].pid=%d \n", i, puntero[i].pid);
        printf("puntero[%d].salida=%d \n", i, puntero[i].salida);
    }

    pipe(tuberia);

    for (int i = 0; i < 2; i++)
    {
        pid = fork();
        if (pid == 0)
        {
            struct sigaction a;
            a.sa_handler = leersalida;
            sigemptyset(&a.sa_mask);
            a.sa_flags = 0;
            sigaction(SIGUSR1, &a, NULL);
            puntero[i].pid = getpid();
            usleep(500);
            for (int j = 0; j < 10; j++)
            {
                printf("Haciendo algo hijo %d \n", getpid());
                sleep(0.5);
            }

            while (puntero[i].salida == 0)
            {
                printf("Esperando a que mi padre me diga que finalice %d \n", getpid());
                sleep(0.5);
            }

            exit(0);
        }
    }

    for (int i = 0; i < 2; i++)
    {
        while (puntero[i].pid == 0)
        {
            usleep(200);
        }
    }

    int valor = 5;
    for (int i = 0; i < 2; i++)
    {
        write(tuberia[1], &valor, sizeof(valor));
        kill(puntero[i].pid, SIGUSR1);
        usleep(200);
    }

    for (int i = 0; i < 2; i++)
    {
        wait(&estado);
    }

    printf("Padre: %d finalizo \n", getpid());

    return 0;
}

void leersalida()
{
    int v;
    read(tuberia[0], &v, sizeof(v));
    printf("Leido %d \n", v);
    for (int i = 0; i < 2; i++)
    {
        if (getpid() == puntero[i].pid)
        {
            puntero[i].salida = 1;
        }
    }
}