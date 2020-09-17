/**************************************************************
 * Title: CIS 452 Lab 2: Simple Shell
 * Authors: Aron Sunuwar & Karan Tamang
 * 
 * Description: This is a simple command line shell that can
 * 		execute some features of a UNIX shell. Can 
 * 		get and parse user inputs, spawn child process
 * 		and execute commands, and can collect and 
 * 		display resource usage statistics about 
 * 		executed process. 
 * ***********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_BUFFER 8000
#define MAX_ARGS 100

void parseInput(char line[], char* argv[]);


int main() {

	char buffer[MAX_BUFFER];
	char* args[MAX_ARGS];
	
	pid_t pid;
	int status;
	struct rusage usage;
	int ret;
	long user_sec;
	long user_usec;
	long nivcsw;
	long prev_user_sec = 0;
	long prev_user_usec = 0;
	long prev_nivcsw = 0;

	// Introduce shell (gives incorrect output on PraireLearn)
	puts("\nWelcome to our Simple Shell!");\
	puts("Enter a command or \"quit\" to exit.\n");
	
	// Loop to keep shell running until quit
	while(1) {
		// Prompt user for a command
		printf("[SimpleShell]$ ");
		
		// Get the user input and save to buffer
		fgets(buffer, MAX_BUFFER, stdin);
		
		// Remove the new line to conduct a clear tokenization
		buffer[strcspn(buffer, "\n")] = 0;
		
		// Exit shell if user enters "quit"
		if(strcmp(buffer, "quit") == 0) {
			printf("Goodbye!\n");
			break;
		}
		
		// Parse the input buffer
		parseInput(buffer, args);
		
		
		// Fork a child to execute the command
		if ((pid = fork()) < 0) {
			perror("fork failure");
			exit(1);
		}
		else if (pid == 0) {
			// Child execute command
			if (execvp(args[0], &args[0]) < 0) {
				perror("exec failed");
				exit(1);
			}

			// Terminate child process
			exit(pid);
		}
		else {
			
			// Parent wait for child to terminate process
			wait(&status);
			
	 		// Retrieve resource usage statistics
	 		ret = getrusage(RUSAGE_CHILDREN, &usage);
	 		
	 		if (ret == -1) {
	 			perror("getrusage failed");
	 		}
	 		
			// Compute User CPU time used and involuntary context switches statistics of each process
			user_sec = usage.ru_utime.tv_sec - prev_user_sec;
			user_usec = usage.ru_utime.tv_usec - prev_user_usec;
			nivcsw = usage.ru_nivcsw - prev_nivcsw;
			
			// Save current User CPU time used and involuntary context switches statistics
			prev_user_sec = usage.ru_utime.tv_sec;
			prev_user_usec = usage.ru_utime.tv_usec;
			prev_nivcsw = usage.ru_nivcsw;
			
			
			//  Print User CPU time used and involuntary context switches
			printf("CPU time used: %ld.%ld us\n", user_sec, user_usec);
			printf("Involuntary context switches: %ld\n", nivcsw);
		}
	}
	
	return 0;
}

void parseInput(char line[], char* argv[]) {

	int i = 0;
	
	// Extract the first token
   	char* token = strtok(line, " ");
   	
   	// Loop through the line to extract all tokens until NULL
   	while(token != NULL) {
   		// Save each token into args
      		argv[i] = token;
      		++i;
      		
      		// Continue to get next token until NULL
      		token = strtok(NULL, " ");
   	}
   	
   	// Add NULL pointer to the end of arguments array
   	argv[i] = NULL;
}




