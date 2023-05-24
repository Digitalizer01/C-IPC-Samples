/*
Create a program that spawns a child process after displaying an identifying message. 
The parent process waits for the child process to terminate.
The child process communicates with the parent process using pipes and synchronization with signals. 
In the initial communication with the child, the parent sends its process identifier (PID) and uses SIGUSR1 as the notification signal.
Then, the child enters a loop with 5 iterations. In each iteration, it sends the index value to the parent and uses SIGUSR2 as the notification signal.
Finally, the child process terminates.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>

int pipefd[2];

void p()
{
    printf("     SIGUSR1 signal received.\n");
    int parent_pid;
    read(pipefd[0], &parent_pid, sizeof(parent_pid));
    printf("     (Function p) Parent. Child PID: %d \n", parent_pid);
    printf("\n");
}

void q()
{
    printf("     SIGUSR2 signal received.\n");
    int index;
    for (int i = 0; i < 5; i++)
    {
        read(pipefd[0], &index, sizeof(index));
        printf("     (Function q) Parent. Message: %d \n", index);
    }
    printf("\n");
}

int main(int argc, char const *argv[])
{
    int child_pid, status;
    int child_process_id;
    pipe(pipefd);

    signal(SIGUSR1, p);
    signal(SIGUSR2, q);

    printf("(PARENT) I am the parent with PID %d\n", getpid());

    child_pid = fork();
    if (child_pid == 0)
    {
        // Child process
        printf("(CHILD) My PID is %d and my parent has PID %d\n", getpid(), getppid());
        printf("\n");

        child_process_id = getpid();
        write(pipefd[1], &child_process_id, sizeof(child_process_id));
        kill(getppid(), SIGUSR1);

        sleep(0.5);

        for (int i = 0; i < 5; i++)
        {
            write(pipefd[1], &i, sizeof(i));
        }

        kill(getppid(), SIGUSR2);

        exit(0);
    }
    else
    {
        wait(&status); // The parent process waits for the child process to terminate.
        // Parent process
        printf("(PARENT) I am the parent with PID %d and I have created my child with PID %d\n", getpid(), child_pid);
    }

    return 0;
}
