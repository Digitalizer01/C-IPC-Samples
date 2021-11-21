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
