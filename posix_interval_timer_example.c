/*
Program that sets up an interval timer using signals. It installs a signal handler for SIGPROF, which is triggered every 3 seconds. Each time the signal is received, it prints an asterisk (*) to the standard error stream. The program will continue running until the condition reaches 5.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include <time.h>

int condition = 0;
struct sigaction action;
struct itimerval timer_value;

static void asteriskHandler(int signal)
{
    write(2, "*", strlen("*")); // Printing an asterisk to stderr
    condition++;
}

void setupSignal()
{
    action.sa_handler = asteriskHandler;
    action.sa_flags = SA_RESTART;
    sigemptyset(&action.sa_mask);
    sigaction(SIGPROF, &action, NULL); // Registering the signal handler for SIGPROF
}

void initializeIntervalTimer()
{
    timer_value.it_interval.tv_sec = 3; // Setting the interval to 3 seconds
    timer_value.it_interval.tv_usec = 0;
    timer_value.it_value = timer_value.it_interval;
    setitimer(ITIMER_PROF, &timer_value, NULL); // Starting the interval timer
}

int main(int argc, char *argv[])
{
    setupSignal();
    initializeIntervalTimer();

    while (condition <= 5)
        ; // Waiting until the condition reaches 5

    exit(0);
}
