/*
Create a C program that spawns a new child process. The parent process displays "I am process XX and I'm going to create a new process" before creating the process. The child process displays the message "I am process YY, my parent is XX", then displays "I am process YY and I'm going to exit" before terminating.
The parent process, after creating the new process, displays the message "Process YY created successfully" and exits.
*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int function1(int a);

int main(int argc, char const *argv[])
{
	// Parent process
	printf("(PARENT) I am process %d and I'm going to create a new process.\n", getpid());

	int pid = fork();

	if (pid == 0)
	{
		// Child process
		printf("(CHILD) I am process %d, my parent is %d.\n", getpid(), getppid());
		printf("(CHILD) I am process %d and I'm going to exit.\n", getpid());

		exit(0);
	}
	else
	{
		// Parent process
		printf("(PARENT) I am %d, process %d created successfully.\n", getpid(), pid);
		sleep(1); // Adding a delay to prevent the parent from terminating before the child. If the parent terminated before the child, the child would show the grandparent as its parent.
		printf("(PARENT) I am %d and I'm going to exit.\n", getpid());

		exit(0);
	}
	return 0;

}