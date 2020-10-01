/**************************************************************
 * Title: CIS 452 Lab 4: Multi-threaded Programming
 * Authors: Aron Sunuwar & Karan Tamang
 * 
 * Description: This is a mini multi-threaded program that 
 		simulates the thread execution manifested by a
 		multi-threaded fileserver process.
 * ***********************************************************/

#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <signal.h>
#include <time.h>

// Declare functions
void* getFile(void* arg);
void sigHandler(int);

int numRequests = 0;
pthread_t threadID;
int status;

/* Main program */
int main() {

	char filename[20];
  
	// Seed time for random generator, only call once
	srand(time(NULL));

	// Install signal handler for interrupt signal
	signal(SIGINT, sigHandler);

	// Run until terminated with Ctrl + C
	while(1) {

		// Get filename as string from user
		printf("\nEnter a filename: \n");
		fgets(filename, 20, stdin);

		// Remove newline and check if input was empty
		filename[strcspn(filename, "\n")] = 0;
		if(filename[0] == 0) {
			printf("FILENAME IS EMPTY!!! TRY AGAIN!\n");
			continue;
		}

		printf("Searching for \"%s\"... Please wait!\n", filename);

		// spawn child thread and communicate filename requested by user
		if ((status = pthread_create(&threadID, NULL,  getFile, &filename)) != 0) {
			fprintf(stderr, "thread create error: %s\n", strerror(status));
			exit (1);
		}
	}

	return 0;
}

/* Prints the diagnostic message with the name of file accessed
 * @param *arg - pointer to filename string */
void* getFile(void* arg) {
	
	char* filename = (char*) arg;
	char* foundAt;

	// Generate random probability number 1 to 10
	int probability = (rand() % 10) + 1;

	// Simulates in disk cache: 80% of the time sleep for 1 sec
	if(probability <= 8) {
		foundAt = "disk cache";
		sleep(1);
	}
	// Simulates not in disk cache: 20% of the time sleep for 7 to 10 secs
	else {
		foundAt = "hard drive";
		sleep(7 + (rand() % 4));
	}

	// Wake up and print diagnostics
	printf("\nFile \"%s\" found in %s.\n", filename, foundAt);

	// Incrementing the number of request
	numRequests++;

	return NULL;
}

/* Signal Handler
 * @param sigNum - signal number for the signal received */
void sigHandler(int sigNum) {

	// Handle signal interrupt: 
	// Print total number of file requests, detach threads and end program
	if(sigNum == SIGINT) {
		puts(" received. That's it, I'm shutting you down...\n");
		printf("Total number of file requests: %d\n", numRequests);

		// Detach the threads
		if ((status = pthread_detach(threadID)) != 0) {
			fprintf (stderr, "detach error %d: %s\n", status, strerror(status));
			exit (1);
		}

		exit(0);
	}
}
