#include <sys/types.h>
#include <signal.h>
#include <stdio.h>

void alarma()
{
    printf("Señal de alarma producida.\n");
    alarm(3);
}

int main(int argc, char const *argv[])
{
    signal(SIGALRM, alarma);
    alarm(3);
    while (1)
        ;
}
