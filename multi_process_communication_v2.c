/*
Program that creates two child processes and communicates with them using shared memory, pipes, and signal notification.
The parent process, after all child processes have been created and completed their tasks, sends them a communication through a pipe and signal to terminate.
The child processes cannot terminate until they receive this signal and activate their exit condition, which is stored in shared memory.
In shared memory, the child processes store their process identifier (PID) as soon as they are created, and the exit condition is initially set to 0. Upon receiving the termination communication, they set the exit condition to 1, allowing them to cleanly terminate their execution.
*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>

void readOutput();

int pipefd[2];

typedef struct
{
    int pid;
    int exit_condition;
} data;

data *pointer;

int main(int argc, char const *argv[])
{

    int status;
    int id;
    int pid;
    key_t key;
    key = ftok("/bin/ls", 76);
    id = shmget(key, 2 * sizeof(data), 0777 | IPC_CREAT); // The '2' indicates the number of fields in the struct
    pointer = (data *)shmat(id, (char *)0, 0);

    for (int i = 0; i < 2; i++)
    {
        pointer[i].pid = 0;
        pointer[i].exit_condition = 0;
        printf("pointer[%d].pid=%d \n", i, pointer[i].pid);
        printf("pointer[%d].exit_condition=%d \n", i, pointer[i].exit_condition);
    }

    pipe(pipefd);

    for (int i = 0; i < 2; i++)
    {
        pid = fork();
        if (pid == 0)
        {
            struct sigaction sa;
            sa.sa_handler = readOutput;
            sigemptyset(&sa.sa_mask);
            sa.sa_flags = 0;
            sigaction(SIGUSR1, &sa, NULL);
            pointer[i].pid = getpid();
            usleep(500);
            for (int j = 0; j < 10; j++)
            {
                printf("Child %d is doing something \n", getpid());
                sleep(0.5);
            }

            while (pointer[i].exit_condition == 0)
            {
                printf("Child %d is waiting for the parent to signal termination \n", getpid());
                sleep(0.5);
            }
            printf("Child %d before terminating \n", getpid());
            exit(0);
        }
    }

    for (int i = 0; i < 2; i++)
    {
        while (pointer[i].pid == 0)
        {
            usleep(200);
        }
    }

    int value = 5;
    for (int i = 0; i < 2; i++)
    {
        write(pipefd[1], &value, sizeof(value));
        kill(pointer[i].pid, SIGUSR1);
        usleep(200);
    }

    for (int i = 0; i < 2; i++)
    {
        wait(&status);
    }

    printf("Parent: %d has finished \n", getpid());

    return 0;
}

void readOutput()
{
    int val;
    read(pipefd[0], &val, sizeof(val));
    printf("Read %d \n", val);
    for (int i = 0; i < 2; i++)
    {
        if (getpid() == pointer[i].pid)
        {
            pointer[i].exit_condition = 1;
        }
    }
}
