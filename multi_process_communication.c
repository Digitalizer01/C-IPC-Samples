/*
Program that creates 5 child processes after displaying an identifying message. The parent process waits until all the children have finished.
Each child process communicates with the parent process using pipes and signal synchronization. The children send their process identification to the parent, then enter a loop with 5 iterations, sending the index value to the parent in each iteration and using SIGUSR2 as a notification.
After that, each child process terminates.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>

int pipefd[2];

void notifyParent();

int main(int argc, char *argv[])
{
    int pid, p, i, status;
    int n;

    if (argc > 1)
        n = atoi(argv[1]);
    else
    {
        printf("Error, execute: ./prs2 num_iterations\n");
        exit(0);
    }

    signal(SIGUSR1, notifyParent);
    pipe(pipefd);

    printf("Parent process: %d, I will create 5 child processes who will send me some data\n", getpid());
    for (i = 0; i < 5; i++)
    {
        pid = fork();
        if (pid == 0)
        {
            p = getpid();
            printf("Child process: %d, I am sending some data to my parent\n", p);

            write(pipefd[1], &p, sizeof(p));
            write(pipefd[1], &n, sizeof(n));
            for (i = 0; i < n; i++)
                write(pipefd[1], &i, sizeof(i));

            kill(getppid(), SIGUSR1);

            printf("Child process finishes\n");
            exit(0);
        }
    }
    printf("Parent process: %d, I will continue when my children finish\n", getpid());
    for (i = 0; i < 5; i++)
        wait(&status);
    printf("Parent process: %d, my children have finished, and I will continue\n", getpid());
    exit(0);
}

void notifyParent()
{
    int p, count, i, j;
    int parent = getpid();
    read(pipefd[0], &p, sizeof(p));
    printf("I am: %d. Received communication from process %d\n", parent, p);
    read(pipefd[0], &count, sizeof(count));
    for (i = 0; i < count; i++)
    {
        read(pipefd[0], &j, sizeof(j));
        printf("I am: %d and I received: %d from process: %d\n", parent, j, p);
    }
}
