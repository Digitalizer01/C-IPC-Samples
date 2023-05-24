/*
Create a C program that demonstrates signal handling. The program registers signal handlers for SIGQUIT and SIGINT.
When a SIGQUIT signal is received, the "q" function is executed, which displays the message "SIGQUIT signal received."
When a SIGINT signal is received, the "p" function is executed, which displays the message "SIGINT signal received" and terminates the program with exit code 2.

The program enters an infinite loop, continuously waiting for signals to be received.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

void q()
{
    printf("SIGQUIT signal received.\n");
    //exit(2);
    // If exit(2) is not present, the program will not terminate until a SIGINT is received.
}

void p()
{
    printf("SIGINT signal received.\n");
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
