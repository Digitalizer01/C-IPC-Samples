/*
Program that sets an alarm signal to be triggered every 3 seconds. When the alarm signal is received, it prints a notification message.
*/

#include <sys/types.h>
#include <signal.h>
#include <stdio.h>

void alarmHandler()
{
    printf("Alarm signal triggered.\n");
    alarm(3); // Resetting the alarm for 3 seconds
}

int main(int argc, char const *argv[])
{
    signal(SIGALRM, alarmHandler); // Registering the signal handler for SIGALRM
    alarm(3); // Setting the initial alarm for 3 seconds

    while (1)
        ; // Infinite loop to keep the program running
}
