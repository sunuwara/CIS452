#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

#define SHM_SIZE 4096
#define TURN 0


// Declare functions
void sigHandler(int);
int shmId;
char *shmPtr;

int main() {

	key_t key;
	char readInput[SHM_SIZE];
	
	// Install signal handler for interrupt signal
	signal(SIGINT, sigHandler);
	
	// Generate a passkey shared to get access to the resource
	if ((key = ftok("/tmp", 'a')) == (key_t) -1) {
    		perror("IPC error: ftok"); 
    		exit(1);
	}
	
	// Create shared memory segment
	if ((shmId = shmget(key, SHM_SIZE + (sizeof(int) * 2), IPC_CREAT|S_IRUSR|S_IWUSR)) < 0) {
		perror("i can't get no..\n");
		exit(1);
	}
	
	// Attach shared memory segment to address space
	if ((shmPtr = shmat(shmId, 0, 0)) == (void*) -1) {
		perror("can't attach\n");
		exit(1);
	}
	
	// Do reading until Ctrl + C
	while(1) {
		
		// shmPtr[SHM_SIZE] = 1 means that its readers turn
		// shmPtr[SHM_SIZE] = 0 means that its writers turn
		
		// Wait until a new string has been written
		while(shmPtr[SHM_SIZE] == 0);
		
		// Read from shmPtr up to its size and print message
		strncpy(readInput, shmPtr, SHM_SIZE);
		printf("Message recieved: %s\n", readInput);
		
		// find some way to check if both readers are done
		// after both readers are done then make turn = 0
		
		// Increment readers count
		shmPtr[SHM_SIZE + sizeof(int)]++;
		printf("Current count: %d\n", shmPtr[SHM_SIZE + sizeof(int)]);
		
		// Wait until both readers read
		while(shmPtr[SHM_SIZE + sizeof(int)] < 2);
		
		printf("New count: %d\n", shmPtr[SHM_SIZE + sizeof(int)]);
		
		// make it writers turn
		shmPtr[SHM_SIZE] = 0;
	}

	return 0;
}

/* Signal Handler
 * @param sigNum - signal number for the signal received */
void sigHandler(int sigNum) {

	// Handle signal interrupt: 
	// Print total number of file requests, detach threads and end program
	if(sigNum == SIGINT) {
		puts(" received. Closing reader...\n");
		
		// Detach shared memory segment from address space
		if (shmdt(shmPtr) < 0) {
			perror ("just can't let go\n");
			exit (1);
		}

		// Destroy the shared memory segment
		if (shmctl(shmId, IPC_RMID, 0) < 0) {
			perror ("can't deallocate\n");
			exit(1);
		}
		
		//free(shmPtr);
		
		exit(0);
	}
}
