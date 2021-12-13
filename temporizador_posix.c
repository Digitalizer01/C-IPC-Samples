#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include <time.h>
int condicion = 0;
struct sigaction accion;
struct itimerval valor;

static void asterisco(int h)
{
    write(2, "*", strlen("*"));
    condicion++;
}
void armado_senial(void)
{
    accion.sa_handler = asterisco;
    accion.sa_flags = SA_RESTART;
    sigemptyset(&accion.sa_mask);
    sigaction(SIGPROF, &accion,
              NULL);
}

void inicializa_intervalo(void)
{
    valor.it_interval.tv_sec = 3;
    valor.it_interval.tv_usec = 0;
    valor.it_value = valor.it_interval;
    setitimer(ITIMER_PROF, &valor, NULL);
}
int main(int argc, char *argv[])
{
    armado_senial();
    inicializa_intervalo();
    while (condicion <= 5)
        ;
    exit(0);
}