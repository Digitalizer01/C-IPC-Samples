/*
This program demonstrates inter-process communication using named pipes in C. The program acts as a reader process that opens a named pipe, reads messages from it until there are no more messages, and then closes and deletes the pipe.

*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
	int pipe_fd;
	char message[100];

	mkfifo("pipe", 0660);  // Create a named pipe
	chmod("pipe", 0660);  // Set permissions on the pipe

	do
	{
		pipe_fd = open("pipe", O_RDONLY);  // Open the named pipe for reading
		if (pipe_fd == -1)
			sleep(1);
	} while (pipe_fd == -1);

	while (read(pipe_fd, message, 100) > 0)  // Read messages from the pipe
	{
		printf("%s\n", message);  // Print the message
	}

	close(pipe_fd);  // Close the pipe
	printf("Reader process finished\n");
	unlink("pipe");  // Delete the named pipe

	return 0;
}