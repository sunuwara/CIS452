#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>


#define READ 		0
#define WRITE 		1
#define MAX_LEN	1024


// Declare functions
void sigHandler(int sigNum);
int getNumChildren(char* userInput);


// Struct of a token that will get passed
typedef struct {
	pid_t destination;
	char message[MAX_LEN];
}Token;

bool end = false;
int pos = 1;
volatile sig_atomic_t eFlag = 1;

int main() 
{	
	int fd[2];
	int numChildren = 0;
	pid_t childPid = 0;
	pid_t mainPid = 0;
	int numPipes = 0;
	int numProcess = 1;	
	ssize_t num;
	char* input = (char*) malloc(sizeof(char) * MAX_LEN);
	Token* token = malloc(sizeof(Token));
	
	
	// Initialize token
	memset(token, 0, sizeof(Token));
	token->destination = -1;
	strcpy(token->message, "");
	
	
	// Install interrupt signal handler
	signal(SIGINT, sigHandler);
	
	
	// Prompt and get the user desired number of children processes to create
	numChildren = getNumChildren(input);
	
	
	// Set up initial pipe
	if (pipe(fd) == -1) {
		perror("pipe failure\n");
		exit(1);
	}
	
	++numPipes;
	printf("INFO: Created a pipe; Total pipes = %d\n", numPipes);
	
	if ((dup2(fd[READ], 3) == -1) || (dup2(fd[WRITE], 4) == -1)) {
		perror("Initial duplicate failure\n");
		exit(1);
	}
	
	close(fd[READ]);
	close(fd[WRITE]);
	
	
	// Spawn children processes and set up pipe for each process
	int pidList[numChildren + 1];
	
	printf("INFO: Spawning %d child processes and initializing Token Ring structure...\n", numChildren);
	for (int i = 0; i < numChildren; i++) { 
		
		// Create additional pipe for each process spawned
		if (pipe(fd) == -1) {
			perror("pipe failure\n");
			exit(1);
		}
		++numPipes;
		printf("INFO: Created a pipe; Total pipes = %d\n", numPipes);
		
		// Spawn child process
		fflush(stdout);
		if ((childPid = fork()) < 0) {
			perror("fork failure\n");
			exit(1);
		}
		++numProcess;
		
		// Child process: Duplicates the new pipe's read
		if (childPid == 0) {
			sleep(2);
			
			printf("INFO: Spawned a new process, PID: %d; Total processes = %d\n", getpid(), numProcess);
			pidList[i + 1] = getpid();
			
			// Resume main process once all the pids are added to the list
			if (i == (numChildren - 1)) {
				kill(mainPid, SIGCONT);
			}
			
			if (dup2(fd[READ], 3) == -1) {
				perror("Child STDIN duplicate failure\n");
				exit(1);
			}
		}
		
		// Parent process: Duplicates the new pipe's write
		else {
			// Save the PID of the main process
			if (i == 0) {
				mainPid = getpid();
				pidList[i] = mainPid;
				printf("INFO: Main PID: %d\n", mainPid);
			}
			
			if (dup2(fd[WRITE], 4) == -1) {
				perror("Parent STDOUT duplicate failure\n");
				exit(1);
			}
		}
		
		// Both process close new pipe's read and write
		close(fd[READ]);
		close(fd[WRITE]);
		
		// Parent process breaks out
		if (childPid != 0) {
			break;
		}
	}

	// Run until SIGINT
	while (eFlag) {
	
		// Main process
		if (getpid() == mainPid) {
			
			// Install continue signal handler
			signal(SIGCONT, sigHandler);
			
			printf("INFO: PID: %d (Main) - waiting...\n", mainPid);
			fflush(stdout);
			
			pause();
			
			// Reading token
			printf("INFO: PID: %d (Main) - reading token\n", mainPid);
			num = read(3, (void *) token, (size_t) sizeof(Token));
			if (num > MAX_LEN) {
				perror("pipe read error\n");
				exit(1);
			}
			
			// Check if message was received
			if (token->destination == 0) {
				printf("INFO: PID: %d (Main) - message was received, resetting message\n", mainPid);
				memset(token->message, 0, strlen(token->message));
			}
			
			// TODO: ERROR HANDLING Prompt for the destination PID
			printf("Select destination PID\n");
			for (int i = 0; i < numChildren + 1; ++i) {
				printf("[%d] - PID %d\n", i, pidList[i]);
			}

			fgets(input, MAX_LEN, stdin);
			
			printf("\n\n%d\n\n", atoi(input));
			token->destination = pidList[atoi(input)];
			
			printf("PID: %d (Main) - Destination PID: %d", mainPid, token->destination);
			
			// TODO: ERROR HANDLING Prompt for the message
			printf("\nPID: %d (Main) - Message: ", mainPid);
			fgets(input, MAX_LEN, stdin);
			strcpy(token->message, input);
			
			
			// Send token to next process
			printf("\nINFO: PID: %d (Main) - sending token\n", mainPid);
			write(4, (const void *) token, (size_t) sizeof(Token));
			
			// Wake up next process
			kill(pidList[pos], SIGCONT);
		}
		
		// Children process
		else {
			childPid = getpid();
			
			// Install continue signal handler
			signal(SIGCONT, sigHandler);
			
			printf("INFO: PID: %d (Child) - waiting...\n", childPid);
			fflush(stdout);
			
			pause();
			
			// Reading token
			printf("INFO: PID: %d (Child) reading token\n", childPid);
			num = read(3, (void *) token, (size_t) sizeof(Token));
			if (num > MAX_LEN) {
				perror("pipe read error\n");
				exit(1);
			}
			sleep(1);
		
			// Check if current process is destination, if destination print message
			if (token -> destination == childPid) {
				printf("PID: %d (Child) - Message received: \n", childPid);
				printf("%s\n", token->message);
				printf("_____________________________________________________\n");
				token->destination = 0;
			}
			sleep(1);
			
			// Send token to next process
			printf("PID: %d (Child) - sending data\n", childPid);
			write(4, (const void *) token, (size_t) sizeof(Token));
			sleep(1);
			
			if (pos == (numChildren)) {
				pos = 0;
			}
			
			// Wake up next process
			printf("Pos = %d\n", pos);
			kill(pidList[pos], SIGCONT);
			
		}
	}
	
	// Deallocate memory
	free(input);
	free(token);
	
	return 0;
}


/* Signal Handler
 * @param sigNum - signal number for the signal received */
void sigHandler(int sigNum)
{	
	// Handle signal interrupt
	if (sigNum == SIGCONT) {
		++pos;
	}
	
	// Handle signal interrupt
	if (sigNum == SIGINT) {		
		puts(" received. I guess we are done here...");
		eFlag = 0;
	}
}


/* Get the number of children processes to create and handle error
 * @param userInput - holds the users input
 * @return numChildren - number of children processes to create */
int getNumChildren(char* userInput)
{
	int numChildren;
	
	while (1) {
		
		// Prompt for the number of children 
		printf("Enter number of processes to create: ");
		fgets(userInput, MAX_LEN, stdin);

		// Remove newline and check if input is valid
		userInput[strcspn(userInput, "\n")] = 0;
		if (userInput[0] == 0) {
			printf("Input failure: empty input\n");
			continue;
		}
		else if ((numChildren = atoi(userInput)) <= 0) {
			printf("Input failure: invalid input\n");
			continue;
		}
		
		break;
	}
	
	return numChildren;
}
