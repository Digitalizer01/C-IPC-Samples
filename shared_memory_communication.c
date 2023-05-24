// Create a child process that communicates using shared memory with two positions, where each position will hold an integer value. The parent process will write to position 0,
// and the child process will write to position 1. Once the shared memory is created, the child process will write the value 5 to its position, wait for one second, display the parent's position,
// modify its position to 5, wait for another second, and then exit.
// The parent process will write the value 7 to its memory position, display the associated position for the child, display the shared memory position associated with the child,
// wait for the child to finish, and then exit.

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

int main(int argc, char *argv[])
{
    int pid_child1 = 0;
    int pidfin, status;
    int e1, e2;

    int identifier;
    int *pointer;

    // Get the key
    key_t key = ftok("/bin/ls", 25);

    // Get the identifier for shared memory
    identifier = shmget(key, 2 * sizeof(int), 0777 | IPC_CREAT);

    // Get the pointer to the first position
    pointer = (int *)shmat(identifier, (char *)0, 0);

    printf("I am the parent process with pid %d\n", getpid());
    pid_child1 = fork(); // Store the return value of the fork call
    if (pid_child1 < 0)
    {
        printf("Error creating the process\n");
        exit(-1); // Exit the program
    }
    else if (pid_child1 == 0)
    { // Code executed only by the new process (child)

        printf("I am the child process with pid %d. My parent is: %d\n", getpid(), getppid());
        sleep(1);

        pointer[1] = 5; // Use it as an array
        sleep(1);
        printf("Value of the shared memory associated with the child: %d\n", pointer[1]);
        sleep(1);
        printf("Value of the shared memory associated with the parent: %d\n", pointer[0]);

        printf("The child process finishes.\n");
        shmdt((char *)pointer); // Release the pointer

        exit(0);
    }
    else
    { // Code executed by the parent process
        printf("Continuing the parent process %d \n", getpid());

        pointer[0] = 7; // Use of shared memory
        sleep(1);
        printf("Value of the shared memory associated with the child: %d\n", pointer[1]);

        // The parent stops using shared memory
        shmdt((char *)pointer); // Release its pointer

        // Wait for the child to finish
        pidfin = wait(&status);
        e1 = status >> 8; // Get the exit value, for example, exit(4)
        e2 = status & 0x00FF;
        if (e2 == 0)
        { // If e2==0, which corresponds to the high part, it means the child terminated by an exit()
            printf("Child process %d finishes with exit(%d)\n", pidfin, e1);
        }
        else
        { // If e2 is not 0, it means the child terminated by a signal
            printf("Child process %d finishes by a signal\n", pidfin);
        }

        // When all children finish, remove the shared memory
        shmctl(identifier, IPC_RMID, 0);

        printf("Parent process finishes\n");
        sleep(1);
        exit(0);
    }
}
