/*
Create a C program that spawns a new child process. The main process, before creating the new process, displays
"I am process XX and I am going to create a new process". The child process displays the message "I am process YY,
my parent is XX", then shows "I am process YY and I am going to exit" and terminates.
Once the new process is created, the parent process displays the message "Process YY created successfully" and exits.
*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int funcion1(int a);

int main(int argc, char const *argv[])
{
    // Parent process
    printf("(PARENT) I am process %d and I am going to create a new process.\n", getpid());
    int pidfin, estado;
    int e1, e2;

    int pid = fork();

    if (pid == 0)
    {
        // Child process
        printf("(CHILD) I am process %d, my parent is %d.\n", getpid(), getppid());
        printf("(CHILD) I am process %d and I am going to exit.\n", getpid());

        exit(9);
    }
    else
    {
        // Parent process
        printf("(PARENT) I am %d, process %d created successfully.\n", getpid(), pid);

        pidfin = wait(&estado); // The parent process waits until the child process terminates.

        // --------------------- PARENT'S TASK STARTS ---------------------
        e1 = estado >> 8;
        e2 = estado & 0x00ff;

        printf("Value of e1: %d \n", e1);
        printf("Value of e2: %d \n", e2);

        if (e2 == 0)
        {
            printf("CHILD process with process ID %d exits with exit(%d) \n", pidfin, e1);
        }
        else
        {
            printf("CHILD process with process ID %d exits due to a signal. \n", pidfin);
        }

        // IMPORTANT: Include one wait statement per child process.

        // ----------------------- PARENT'S TASK ENDS ----------------------

        printf("(PARENT) I am %d and I am going to exit.\n", getpid());

        exit(0);
    }
    return 0;
}
