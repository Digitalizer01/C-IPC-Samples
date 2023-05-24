## C Scripts for Multi-Process Communication and Signal Handling

This repository contains a collection of C scripts focused on multi-process communication, pipe handling, shared memory communication, signal handling, and alarms. These scripts showcase various aspects of inter-process communication and signal management in C programming.

# Repository Contents

# Child Process Creation and Termination

create_child_process.c demonstrates the creation of a new child process. The main process, before creating the new process, displays the message "I am process XX and I am going to create a new process". The child process then displays its own process ID and the parent's process ID, followed by the message "I am process YY and I am going to exit" before terminating. Once the new process is created, the parent process displays the message "Process YY created successfully" and exits.

## Script Explanation

The C script can be summarized as follows:

1. The parent process begins by printing its own process ID along with a message indicating the creation of a new process.
2. A fork system call is used to create a new child process.
3. If the fork call returns 0, it means the code is executing in the child process. The child process prints its process ID and the parent's process ID, and then exits with an exit code of 9.
4. If the fork call returns a positive value (process ID of the child), it means the code is executing in the parent process. The parent process prints its process ID and the child's process ID and proceeds with the following tasks:
   - It waits for the child process to terminate using the `wait` system call, storing the child's process ID and termination status in `pidfin` and `estado`, respectively.
   - The termination status is then analyzed to determine if the child process exited normally or due to a signal.
   - If the child process exited normally, the exit code is extracted from `estado` and displayed along with the child's process ID.
   - If the child process exited due to a signal, a corresponding message is displayed.
5. Finally, the parent process prints its own process ID and exits with an exit code of 0.

# Child Process Communication with Pipes and Signal Synchronization

multi_process_communication.c creates 5 child processes after displaying an identifying message. The parent process waits until all the children have finished. Each child process communicates with the parent process using pipes and signal synchronization.

The program can be summarized as follows:

1. The command-line argument `num_iterations` specifies the number of iterations for each child process. The program expects the argument to be passed when executing the script.
2. A pipe is created using the `pipe` system call to enable communication between the parent and child processes.
3. The parent process displays its process ID and announces the creation of 5 child processes.
4. In a loop, the parent process forks and creates a child process. Each child process performs the following tasks:
   - It retrieves its own process ID using `getpid()` and sends it to the parent process through the pipe.
   - It sends the value of `num_iterations` to the parent process.
   - It enters a loop with `num_iterations` iterations and sends the current index value to the parent process through the pipe.
   - After each iteration, it uses the `SIGUSR1` signal to notify the parent process of new data.
   - Finally, the child process terminates.
5. The parent process displays a message indicating that it will wait for the child processes to finish.
6. The parent process waits for each child process to finish using the `wait` system call.
7. Once all child processes have finished, the parent process displays a message indicating that the children have finished, and it terminates.

Note: The communication between child and parent processes is achieved using pipes, allowing the exchange of process identification and data. The `SIGUSR1` signal is used for synchronization between the child and parent processes.

# Shared Memory Communication in C

shared_memory_communication.c demonstrates the communication between a parent process and a child process using shared memory. The parent process creates a child process and establishes shared memory with two positions. Each process writes to its respective position in shared memory.

## Parent Process

1. The parent process displays its process ID.
2. It forks a child process using `fork()`.
3. If the `fork()` call fails, an error message is displayed, and the program exits.
4. If the `fork()` call succeeds, the parent process continues.
5. The parent process writes the value 7 to its position in shared memory.
6. It waits for the child to finish using `wait()`.
7. The parent process retrieves the exit status of the child process and displays whether it terminated by an exit code or a signal.
8. The parent process releases its pointer to shared memory.
9. Finally, the parent process removes the shared memory using `shmctl()` and exits.

## Child Process

1. The child process displays its process ID and the ID of its parent.
2. It sleeps for 1 second.
3. The child process writes the value 5 to its position in shared memory.
4. It sleeps for another second.
5. The child process displays the value of shared memory associated with itself and the parent process.
6. Finally, the child process releases its pointer to shared memory, exits, and terminates.

# Interprocess Communication with Shared Memory, Pipes, and Signals in C

multi_process_communication_v2.c demonstrates interprocess communication between a parent process and two child processes using shared memory, pipes, and signals.

## Parent Process

1. The parent process initializes shared memory using `shmget()` and attaches to it using `shmat()`.
2. Two fields are defined in shared memory for each child process: `pid` and `exit_condition`.
3. The parent process creates a pipe using `pipe()` to communicate with the child processes.
4. It then forks two child processes.
5. Each child process sets up a signal handler using `sigaction()` to handle the `SIGUSR1` signal.
6. The child process stores its PID in the shared memory and waits for a short period.
7. The child process enters a loop, performing some work and sleeping for 0.5 seconds.
8. Meanwhile, the child process checks the `exit_condition` in shared memory. If it is 0, the child process continues to wait.
9. When the `exit_condition` is set to 1 by the parent process, the child process prints a termination message and exits.
10. The parent process waits for each child process to initialize its PID in shared memory.
11. Next, the parent process writes a value (`5`) to the pipe for each child process and sends the `SIGUSR1` signal to each child process.
12. The parent process waits for each child process to terminate using `wait()`.
13. Finally, the parent process displays a completion message and exits.

## Child Processes

1. Each child process initializes its signal handler to handle the `SIGUSR1` signal.
2. The child process stores its PID in the shared memory and waits for a short period.
3. The child process enters a loop, performing some work and sleeping for 0.5 seconds.
4. The child process checks the `exit_condition` in shared memory. If it is 0, the child process continues to wait.
5. When the `exit_condition` is set to 1 by the parent process, the child process prints a termination message and exits.

Note: The communication between the parent and child processes is facilitated through shared memory and pipes. The `SIGUSR1` signal is used for synchronization and termination notification.

# Interprocess Communication using Named Pipes in C

named_pipe_writer.c demonstrates interprocess communication using named pipes. The program acts as a writer process that opens a named pipe, writes a message to it three times at regular intervals, and then closes the pipe.
named_pipe_reader.c acts as a reader process in interprocess communication using named pipes. The reader process opens a named pipe, reads messages from it until there are no more messages, and then closes and deletes the pipe.

## Writer Process

1. The writer process initializes the `pipe_fd` variable and the `message` array.
2. The writer process formats a greeting message using `sprintf()`, including its own process ID.
3. The writer process enters a loop to open the named pipe for writing.
4. If the open operation fails (`pipe_fd == -1`), the process sleeps for 1 second and retries.
5. Once the named pipe is successfully opened, the writer process proceeds to write the message to the pipe three times.
6. Inside the loop, the writer process uses `write()` to send the message to the named pipe.
7. After writing the message, the process sleeps for 2 seconds using `sleep()` to simulate an interval.
8. Once the loop completes, the writer process closes the named pipe using `close()`.
9. Finally, the process prints a completion message and exits.

Note: The writer process repeatedly opens the named pipe until it is successful. This is because the reader process may not have created the pipe yet. The writer process blocks until the reader process opens the named pipe.

## Reader Process

1. The reader process initializes the `pipe_fd` variable and the `message` array.
2. The reader process creates a named pipe using `mkfifo()` with the name "pipe" and sets the permissions using `chmod()`.
3. The reader process enters a loop to open the named pipe for reading.
4. If the open operation fails (`pipe_fd == -1`), the process sleeps for 1 second and retries.
5. Once the named pipe is successfully opened, the reader process reads messages from the pipe using `read()` inside a `while` loop.
6. The `read()` function reads up to 100 bytes from the pipe into the `message` array. If there are no more messages in the pipe, the `read()` function returns 0 and the loop terminates.
7. Inside the loop, the reader process prints the message using `printf()`.
8. After reading all the messages, the reader process closes the named pipe using `close()`.
9. The process prints a completion message, and then deletes the named pipe using `unlink()`.

Note: The reader process repeatedly opens the named pipe until it is successful. This is because the writer process may not have created the pipe yet. The reader process blocks until the writer process opens the named pipe.

# Interval Timer using Signals

posix_interval_timer_example.c sets up an interval timer using signals. It installs a signal handler for SIGPROF, which is triggered every 3 seconds. Each time the signal is received, it prints an asterisk (*) to the standard error stream. The program continues running until the `condition` variable reaches 5.

## Signal Handler

1. The program defines a signal handler function named `asteriskHandler` that takes a signal as an argument.
2. Inside the `asteriskHandler` function, it uses `write()` to print an asterisk (*) to the standard error stream (`stderr`).
3. The `condition` variable is incremented each time the signal handler is called.

## Signal Setup

1. The program defines a function named `setupSignal` that sets up the signal handling.
2. It initializes the `action` struct with the signal handler function and sets the `SA_RESTART` flag.
3. The `sigaction()` function is called to register the signal handler function (`asteriskHandler`) for the `SIGPROF` signal.

## Interval Timer Initialization

1. The program defines a function named `initializeIntervalTimer` that sets up the interval timer.
2. It initializes the `timer_value` struct with the interval duration of 3 seconds.
3. The `setitimer()` function is called with the `ITIMER_PROF` parameter to start the interval timer.

## Main Function

1. In the `main()` function, the `setupSignal()` and `initializeIntervalTimer()` functions are called to set up the signal handling and start the interval timer.
2. The program enters a loop and continues running until the `condition` variable reaches 5.
3. Once the `condition` reaches 5, the loop exits and the program terminates by calling `exit(0)`.

Notes:
- The program uses `write(2, "*", strlen("*"))` to print an asterisk (*) to the standard error stream (`stderr`) because the standard output stream (`stdout`) is typically line-buffered, and it may not immediately display the output.
- This program demonstrates how to set up an interval timer using signals. It utilizes the SIGPROF signal and a signal handler function to print an asterisk (*) to the standard error stream every 3 seconds. The program continues running until the condition variable reaches 5.

# Communication Between Parent and Child Processes using Pipes and Signals

process_communication.c spawns a child process after displaying an identifying message. The parent process waits for the child process to terminate. The child process communicates with the parent process using pipes and synchronization with signals.

## Signal Handlers

1. The program defines two signal handler functions: `p()` and `q()`.
2. The `p()` function is registered as the signal handler for `SIGUSR1`. When `SIGUSR1` is received, the function is executed.
3. Inside `p()`, the parent process reads the parent's process identifier (PID) from the pipe and prints it.
4. The `q()` function is registered as the signal handler for `SIGUSR2`. When `SIGUSR2` is received, the function is executed.
5. Inside `q()`, the child process reads an index value from the pipe and prints it. This loop repeats five times.

## Main Function

1. In the `main()` function, a pipe is created using `pipe()`. The pipe has two file descriptors: `pipefd[0]` for reading and `pipefd[1]` for writing.
2. The signal handlers `p()` and `q()` are registered for `SIGUSR1` and `SIGUSR2` signals, respectively.
3. The parent process displays its PID and then forks to create a child process.
4. If `fork()` returns 0, it means the current process is the child process.
5. In the child process block, the child process displays its own PID and its parent's PID.
6. The child process writes its PID to the pipe and sends a `SIGUSR1` signal to the parent process.
7. After a short delay (`sleep(0.5)`), the child process enters a loop and writes an index value to the pipe five times.
8. After each write, the child process sends a `SIGUSR2` signal to the parent process.
9. Finally, the child process exits.
10. In the parent process block, the parent process waits for the child process to terminate using `wait(&status)`.
11. After the child process terminates, the parent process displays its own PID and the PID of the child process.

Notes:

- The `SIGUSR1` and `SIGUSR2` signals are user-defined signals that can be used for communication between processes.
- This program demonstrates communication between a parent process and a child process using pipes and synchronization with signals. The parent process spawns a child process, and they exchange information through a pipe. The parent process waits for the child process to terminate, and then both processes display their respective process identifiers (PIDs).

# Communication Between Parent and Child Processes using Pipes and Signal Synchronization

process_communication_v2.c creates a child process after displaying an identifying message. The parent process waits until the child process finishes. The child process communicates with the parent process using pipes and signal synchronization.

## Signal Handlers

1. The program defines two signal handler functions: `notifyParent()` and `notifyParent2()`.
2. The `notifyParent()` function is registered as the signal handler for `SIGUSR1`. When `SIGUSR1` is received, the function is executed.
3. Inside `notifyParent()`, the parent process reads the child's process identifier (PID) from the pipe and prints it.
4. The `notifyParent2()` function is registered as the signal handler for `SIGUSR2`. When `SIGUSR2` is received, the function is executed.
5. Inside `notifyParent2()`, the parent process reads a count value from the pipe and prints a series of index values. This loop repeats `count` times.

## Main Function

1. In the `main()` function, a pipe is created using `pipe()`. The pipe has two file descriptors: `pipefd[0]` for reading and `pipefd[1]` for writing.
2. The signal handlers `notifyParent()` and `notifyParent2()` are registered for `SIGUSR1` and `SIGUSR2` signals, respectively.
3. The program checks if the command-line argument `argv[1]` is provided, which represents the number of iterations.
4. The parent process displays its PID and then forks to create a child process.
5. If `fork()` returns 0, it means the current process is the child process.
6. In the child process block, the child process displays its own PID and writes its PID to the pipe.
7. After writing the PID, the child process sends a `SIGUSR1` signal to the parent process.
8. After a short delay (`sleep(0.5)`), the child process writes the value of `n` (number of iterations) to the pipe.
9. Then, the child process enters a loop and writes index values to the pipe `n` times.
10. After each write, the child process sends a `SIGUSR2` signal to the parent process.
11. Finally, the child process exits.
12. In the parent process block, the parent process waits for the child process to terminate using `wait(&status)`.
13. After the child process terminates, the parent process displays its own PID and continues execution.

Notes:
- The `SIGUSR1` and `SIGUSR2` signals are user-defined signals that can be used for communication between processes.
- This program demonstrates communication between a parent process and a child process using pipes and signal synchronization. The parent process creates a child process, and they exchange information through a pipe. The parent process waits for the child process to finish, and then both processes display their respective process identifiers (PIDs).

# Parent-Child Process Relationship

process_creation.c spawns a new child process, and the parent and child processes exchange messages before terminating.

## Main Function

1. In the `main()` function, the parent process starts by printing its process ID (PID) and the message "I am process XX and I'm going to create a new process" using `printf()`.
2. The program calls `fork()` to create a new child process. The `fork()` function returns 0 in the child process and the child's PID in the parent process.
3. If `fork()` returns 0, indicating that the current process is the child process, the child process block is executed.
4. Inside the child process block, the child process prints its own PID, parent's PID (using `getppid()`), and the message "I am process YY, my parent is XX" using `printf()`.
5. Then, the child process prints "I am process YY and I'm going to exit" and terminates using `exit(0)`.
6. If `fork()` returns a non-zero value (PID of the child) in the parent process block, the parent process executes.
7. Inside the parent process block, the parent process prints its own PID, the child's PID, and the message "Process YY created successfully" using `printf()`.
8. A short delay of 1 second (`sleep(1)`) is added to ensure that the parent process doesn't terminate before the child process. This delay allows the child process to complete its execution.
9. Finally, the parent process prints "I am process XX and I'm going to exit" and terminates using `exit(0)`.

Notes:
- The values XX and YY represent the process IDs of the parent and child processes, respectively.
- This program demonstrates the creation of a child process by a parent process. The parent process displays its own PID and the intention to create a new process. After forking, the child process displays its own PID and the parent's PID. Then, the child process prints a termination message and exits. Meanwhile, the parent process displays the child's PID and a success message before terminating.

# Alarm Signal

alarm_example.c sets an alarm signal to be triggered every 3 seconds. When the alarm signal is received, it prints a notification message.

## Alarm Handler Function

1. The `alarmHandler()` function is defined, which will be called when the alarm signal (SIGALRM) is received.
2. Inside the `alarmHandler()` function, it prints the message "Alarm signal triggered." using `printf()`.
3. It then calls `alarm(3)` to reset the alarm for another 3 seconds, ensuring that the alarm signal will be triggered again after 3 seconds.

## Main Function

1. In the `main()` function, the program registers the signal handler `alarmHandler()` for the SIGALRM signal using `signal(SIGALRM, alarmHandler)`.
2. It then sets the initial alarm using `alarm(3)` to trigger the alarm signal after 3 seconds.
3. The program enters an infinite loop (`while(1)`) to keep the program running indefinitely.

Note:
- The alarm signal (SIGALRM) is a signal that is triggered by the operating system after a specified amount of time has passed. In this program, the alarm signal is set to be triggered every 3 seconds.
- This program demonstrates the use of the alarm signal (SIGALRM) in C. It sets an alarm to be triggered every 3 seconds using the alarm() function. When the alarm signal is received, the program executes the alarm handler function (alarmHandler()), which prints a notification message and resets the alarm for another 3 seconds. The program continues running in an infinite loop to ensure that the alarm signal is repeatedly triggered.

# Signal Handling

signal_handling.c demonstrates signal handling. It registers signal handlers for SIGQUIT and SIGINT signals.

## Signal Handlers

1. The program defines two signal handler functions: `q()` and `p()`.
2. The `q()` function is associated with the SIGQUIT signal. When a SIGQUIT signal is received, it is executed and it prints the message "SIGQUIT signal received" using `printf()`.
3. The `p()` function is associated with the SIGINT signal. When a SIGINT signal is received (typically triggered by pressing Ctrl+C in the terminal), it is executed and it prints the message "SIGINT signal received" using `printf()`. Additionally, it terminates the program with an exit code of 2 using `exit(2)`.

## Main Function

1. In the `main()` function, the program registers the signal handler `q()` for the SIGQUIT signal using `signal(SIGQUIT, q)`.
2. It also registers the signal handler `p()` for the SIGINT signal using `signal(SIGINT, p)`.
3. The program enters an infinite loop (`while(1)`) to continuously wait for signals to be received.
4. As long as the program is running, it will respond to received signals by executing the associated signal handler functions.

Notes:
- The SIGQUIT signal is typically generated by the user pressing Ctrl+\ in the terminal. The SIGINT signal is generated by the user pressing Ctrl+C in the terminal.
Certainly! Here's an explanation of the provided C script that demonstrates signal handling:

markdown

# Signal Handling

signal_handling.c demonstrates signal handling. It registers signal handlers for SIGQUIT and SIGINT signals.

## Signal Handlers

1. The program defines two signal handler functions: `q()` and `p()`.
2. The `q()` function is associated with the SIGQUIT signal. When a SIGQUIT signal is received, it is executed and it prints the message "SIGQUIT signal received" using `printf()`.
3. The `p()` function is associated with the SIGINT signal. When a SIGINT signal is received (typically triggered by pressing Ctrl+C in the terminal), it is executed and it prints the message "SIGINT signal received" using `printf()`. Additionally, it terminates the program with an exit code of 2 using `exit(2)`.

## Main Function

1. In the `main()` function, the program registers the signal handler `q()` for the SIGQUIT signal using `signal(SIGQUIT, q)`.
2. It also registers the signal handler `p()` for the SIGINT signal using `signal(SIGINT, p)`.
3. The program enters an infinite loop (`while(1)`) to continuously wait for signals to be received.
4. As long as the program is running, it will respond to received signals by executing the associated signal handler functions.

Note:
- The SIGQUIT signal is typically generated by the user pressing Ctrl+\ in the terminal. The SIGINT signal is generated by the user pressing Ctrl+C in the terminal.
- This program demonstrates the handling of SIGQUIT and SIGINT signals in C. It registers signal handlers q() and p() for SIGQUIT and SIGINT signals, respectively. When a SIGQUIT signal is received, the q() function is executed and prints a message. When a SIGINT signal is received, the p() function is executed, prints a message, and terminates the program with an exit code of 2.
- The program enters an infinite loop, continuously waiting for signals to be received. It will respond to received signals by executing the associated signal handler functions.

# Signal Handling

signal_handling_v2.c handles signals SIGQUIT and SIGINT.

## Signal Handlers

1. The program defines two signal handler functions: `handleSIGQUIT()` and `handleSIGINT()`.
2. The `handleSIGQUIT()` function is associated with the SIGQUIT signal (generated by pressing Ctrl+\ in the terminal). When a SIGQUIT signal is received, it is executed, and it prints the message "Received SIGQUIT signal" using `printf()`.
3. The `handleSIGINT()` function is associated with the SIGINT signal (generated by pressing Ctrl+C in the terminal). When a SIGINT signal is received, it is executed, and it prints the message "Received SIGINT signal" using `printf()`. Additionally, it terminates the program with an exit code of 2 using `exit(2)`.

## Main Function

1. In the `main()` function, the program sets up signal handling for SIGQUIT and SIGINT.
2. It creates `struct sigaction` objects `sigquitAction` and `sigintAction` to define the signal handling behavior.
3. For each signal, it assigns the corresponding signal handler function and sets the `sa_mask` and `sa_flags` fields.
4. It uses `sigaction()` to register the signal handling behavior for SIGQUIT and SIGINT using the `sigquitAction` and `sigintAction` objects, respectively.
5. The program enters an infinite loop (`while(1)`) and continuously waits for signals to be received.
6. Within the loop, it prints the message "Waiting for signal and working" using `printf()` and sleeps for 1 second using `sleep(1)`.

Notes:
- The SIGQUIT signal is typically generated by the user pressing Ctrl+\ in the terminal. The SIGINT signal is generated by the user pressing Ctrl+C in the terminal.
- This program handles the SIGQUIT and SIGINT signals in C. It defines signal handler functions handleSIGQUIT() and handleSIGINT(). When a SIGQUIT signal (generated by pressing Ctrl+) is received, the handleSIGQUIT() function is executed and prints a message. When a SIGINT signal (generated by pressing Ctrl+C) is received, the handleSIGINT() function is executed, prints a message, and terminates the program with an exit code of 2.
- The program sets up the signal handling behavior using struct sigaction objects. It registers the signal handlers for SIGQUIT and SIGINT using sigaction(). The program then enters an infinite loop, continuously waiting for signals to be received. Within the loop, it prints a message and sleeps for 1 second.
