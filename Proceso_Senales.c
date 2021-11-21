#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

void q()
{
    printf("Se ha producido la señal SIGQUIT\n");
    //exit(2);
    // Si exit(2) no está, el programa no se detendrá hasta que se tenga SIGINT.
}

void p()
{
    printf("Se ha producido la señal SIGINT\n");
    exit(2);
}

int main(int argc, char const *argv[])
{
    signal(SIGQUIT, q);
    signal(SIGINT, p);

    while (1)
        ;

    return 0;
}
