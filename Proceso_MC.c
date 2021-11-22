//Crear un proceso hijo que se comunique con memoria compartida de dos posiciones, donde cada posicion va a ser un valor entero, el proceso padre escribira en la posicion 0
//y el hijo escribira en la posicion 1, una vez creada la memoria compartida, se creara el proceso hijo, el proceso hijo escribira un valor 5 es su posicion, esperara un segundo, visualiza la posicion del padre, modifica su posicion a 5, espera otro segundo y finalizara.
//el proceso padre escribe un valor 7 en su posicion de memoria, visualiza la posicion asociada al hijo, visualiza la posicion de memoria asociada al hijo,
//espera a que el hijo finalice y finaliza.

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

int main(int argc, char *argv[])
{
    int pid_hijo1 = 0;
    int pidfin, estado;
    int e1, e2;

    int identificador;
    int *puntero;

    //Obtener la clave
    key_t clave = ftok("/bin/ls", 25);

    // Obtener el identificador a memoria compartida
    identificador = shmget(clave, 2 * sizeof(int), 0777 | IPC_CREAT);

    // Obtener el puntero a la primera posición
    puntero = (int *)shmat(identificador, (char *)0, 0);

    printf("Soy el proceso padre con pid %d\n", getpid());
    pid_hijo1 = fork(); //se almacena el valor de retorno de la llamada fork
    if (pid_hijo1 < 0)
    {
        printf("Error en la cereacion del proceso\n");
        exit(-1); //salir del programa
    }
    else if (pid_hijo1 == 0)
    { //codigo que ejecuta solo el nuevo proceso (hijo)

        printf("Soy el proceso hijo con pid %d. Mi padre es: %d\n", getpid(), getppid());
        sleep(1);

        puntero[1] = 5; // utilizar como un array
        sleep(1);
        printf("Valor de la memoria compartida asociado al hijo %d\n", puntero[1]);
        sleep(1);
        printf("Valor de la memoria compartida asociado al padre %d\n", puntero[0]);

        printf("El hijo finaliza.\n");
        shmdt((char *)puntero); //libera el puntero

        exit(0);
    }
    else
    { //Parte que ejecuta el proceso padre
        printf("Continua el proceso padre %d \n", getpid());

        puntero[0] = 7; //Uso de memoria compartida
        sleep(1);
        printf("Valor de la memoria compartida asociado al hijo %d\n", puntero[1]);

        //El padre deja de utilizar memoria compartida
        shmdt((char *)puntero); // libera su puntero

        // Espera a que termine el hijo

        pidfin = wait(&estado);
        e1 = estado >> 8; //Devuelve el valor del exit por ejemplo exit(4)
        e2 = estado & 0x00FF;
        if (e2 == 0)
        { //Si e2==0 que se corresponde con la parte alta es porque ha terminado por un exit()
            printf("Proceso hijo %d finaliza por exit(%d)\n", pidfin, e1);
        }
        else
        { //Si e2 no es 0 es porque ha terminado por una señal
            printf("Proceso hijo %d finaliza por una señal\n", pidfin);
        }

        //Cuando todos los hijos terminan elimina la memoria compartida
        shmctl(identificador, IPC_RMID, 0);

        printf("Padre finaliza\n");
        sleep(1);
        exit(0);
    }
}