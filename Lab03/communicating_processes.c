cle/**************************************************************
 * Title: CIS 452 Lab 3: IPC Communicating Processes
 * Authors: Aron Sunuwar & Karan Tamang
 * 
 * Description: Asynchronous communication is an IPC technique
 * 		often used in systems programming, where
 *		processes mush be able to respond to a variety 
 *		of possible inputs and events. This program is
 *		used to experiment with asynchronous IPC by 
 *		writing a program that demonstrates the use of
 *		signal for simple communication.
 * ***********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

// Declare functions
void sigHandler(int);

/* Main program
*/
int main() {

	pid_t child_pid;
	pid_t parent_pid;
	
	// Seed time for random generator, only call once
	srand(time(NULL));


	// Spawn a child process
	if((child_pid = fork()) < 0) {
		perror("fork failure");
		exit(1);
	}
	else if(child_pid == 0) {
		// Child process gets the parent pid
		parent_pid = getppid();
	}
	else {
		// Parent process prints childs pid
		printf("spawned child PID# %d\n", child_pid);
	}
	
	// Run until SIGINT
	while(1) {
		
		// Child process
		if(child_pid == 0) {
			
			// Wait a random amount of time 1 to 5 secs
			sleep((rand() % 5) + 1);
			
			// Randomly select user-defined signal to parent
			if(((rand() % 2) + 1) == 1) {
				kill(parent_pid, SIGUSR1);
			}
			else {
				kill(parent_pid, SIGUSR2);
			}
			
		}
		
		// Parent process
		else {
		
			// Install signal handler
			signal(SIGUSR1, sigHandler);
			signal(SIGUSR2, sigHandler);
			signal(SIGINT, sigHandler);
			
			printf("waiting...\t");
			fflush(stdout);
			
			pause();
		}
	}
	
	return 0;
}

/* Signal Handler
 * @param sigNum - signal number for the signal received */
void sigHandler(int sigNum) {
	
	// Handle user defined signal 1
	if(sigNum == SIGUSR1) {
		puts(" received a SIGUSR1 signal");
	}
	
	// Handle user defined signal 2
	if(sigNum == SIGUSR2) {
		puts(" received a SIGUSR2 signal");
	}
	
	// Handle signal interrupt
	if(sigNum == SIGINT) {
		puts(" received. That's it, I'm shutting you down...");
		exit(0);
	}
}
