/*
Program that handles signals SIGQUIT and SIGINT in C.
When SIGQUIT signal (Ctrl+\) is received, it prints a message.
When SIGINT signal (Ctrl+C) is received, it exits the program with exit code 2.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

void handleSIGQUIT()
{
    printf("Received SIGQUIT signal\n");
    // exit(2);
    // If exit(2) is commented, the program will not terminate until a SIGINT signal is received.
}

void handleSIGINT()
{
    printf("Received SIGINT signal\n");
    exit(2);
}

int main(int argc, char *arg[])
{
    // Handle SIGQUIT signal (Ctrl+\)
    struct sigaction sigquitAction;
    sigquitAction.sa_handler = handleSIGQUIT;
    sigemptyset(&sigquitAction.sa_mask);
    sigquitAction.sa_flags = 0;
    sigaction(SIGQUIT, &sigquitAction, NULL);

    // Handle SIGINT signal (Ctrl+C)
    struct sigaction sigintAction;
    sigintAction.sa_handler = handleSIGINT;
    sigemptyset(&sigintAction.sa_mask);
    sigintAction.sa_flags = 0;
    sigaction(SIGINT, &sigintAction, NULL);

    while (1)
    {
        sleep(1);
        printf("Waiting for signal and working\n");
    }

    return 0;
}
