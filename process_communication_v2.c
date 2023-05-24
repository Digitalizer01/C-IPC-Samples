/*
Program that creates a child process after displaying an identifying message. The parent process waits until the child process finishes.
The child process communicates with the parent process using pipes and signal synchronization. In the initial communication, the child sends its process identification to the parent and uses the SIGUSR1 signal as a notification. Then, it enters a loop with 5 iterations, sending the index value to the parent in each iteration and using the SIGUSR2 signal as a notification.
After that, the child process terminates.
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
void notifyParent2();

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
    signal(SIGUSR2, notifyParent2);
    pipe(pipefd);

    printf("Parent process: %d, I will create a child who will send me some data\n", getpid());
    pid = fork();

    if (pid == 0)
    {
        p = getpid();
        printf("Child process: %d, I am sending some data to my parent\n", p);

        write(pipefd[1], &p, sizeof(p));
        kill(getppid(), SIGUSR1);
        sleep(0.5);

        write(pipefd[1], &n, sizeof(n));
        for (i = 0; i < n; i++)
            write(pipefd[1], &i, sizeof(i));

        kill(getppid(), SIGUSR2);
        sleep(0.5);

        printf("Child process finishes\n");
        exit(0);
    }

    printf("Parent process: %d, I will continue when my child finishes\n", getpid());
    wait(&status);
    printf("Parent process: %d, my child has finished, and I will continue\n", getpid());
    exit(0);
}

void notifyParent()
{
    int p;
    int parent = getpid();
    read(pipefd[0], &p, sizeof(p));
    printf("I am: %d. Received communication from process %d\n", parent, p);
}

void notifyParent2()
{
    int count, i, j;
    int parent = getpid();
    read(pipefd[0], &count, sizeof(count));
    for (i = 0; i < count; i++)
    {
        read(pipefd[0], &j, sizeof(j));
        printf("I am: %d and I received: %d\n", parent, j);
    }
}
