/*
This program demonstrates inter-process communication using named pipes in C. The program acts as a writer process that opens a named pipe, writes a message to it three times at regular intervals, and then closes the pipe.

*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
	int pipe_fd, i;
	char message[100];
	sprintf(message, "Greetings from the writer process with pid= %d\n", getpid());
	pipe_fd = -1;

	do
	{
		pipe_fd = open("pipe", O_WRONLY);  // Open the named pipe for writing
		if (pipe_fd == -1)
			sleep(1);
	} while (pipe_fd == -1);

	for (i = 0; i < 3; i++)
	{
		write(pipe_fd, message, 100);  // Write the message to the pipe
		sleep(2);  // Wait for 2 seconds
	}

	close(pipe_fd);  // Close the pipe
	printf("Writer process finished\n");

	return 0;
}