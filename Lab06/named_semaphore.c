/**************************************************************
 * Title: CIS 452 Lab 6: Shared Memory Synchronization
 * Authors: Aron Sunuwar & Karan Tamang
 * 
 * Description: This is a program that demonstrates the proper
 		synchronization to protect critical sections
 		using named semaphore.
 * ***********************************************************/
 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>

#define SIZE 16

int main(int argc, char *argv[]) {

	int status;
	long int i, loop, temp, *shmPtr;
	int shmId;
	pid_t pid;

	// Get value of loop variable (from command-line argument)
	if(argc != 2) {
		perror("invalid number of arguments\n");
		exit(1);
	}
	loop = atoi(argv[1]);
	printf("Loop = %ld\n", loop);
	
	// Create a shared memory segment
	if((shmId = shmget(IPC_PRIVATE, SIZE, IPC_CREAT|S_IRUSR|S_IWUSR)) < 0) {
		perror("i can't get no..\n");
		exit(1);
	}
	
	// Attach shared memory segment to address space 
	if((shmPtr = shmat(shmId, 0, 0)) == (void*) -1) {
		perror("can't attach\n");
		exit(1);
	}

	// Initial values for shared data
	shmPtr[0] = 0;
	shmPtr[1] = 1;
	printf("Initial values: shmPtr[0] = %li\tshmPtr[1] = %li\n", shmPtr[0], shmPtr[1]);

	// Fork child, both child and parent conduct own swapping 
	if(!(pid = fork())) {
	
		// Child process
		for(i = 0; i < loop; i++) {
		
			// Critical section --> swap the contents of shmPtr[0] and shmPtr[1]
			temp = shmPtr[0];
			shmPtr[0] = shmPtr[1];
			shmPtr[1] = temp;
			// End Critical section
		}
		
		if(shmdt(shmPtr) < 0) {
			perror("just can't let go\n");
			exit(1);
		}
		
		exit(0);
	}
	else {
		
		// Parent Process
		for(i = 0; i < loop; i++) {
		
			// Critical section --> swap the contents of shmPtr[0] and shmPtr[1]
			temp = shmPtr[0];
			shmPtr[0] = shmPtr[1];
			shmPtr[1] = temp;
			// End Critical section
		}
	}

	// Parent waits for child to finish and print the final output
	wait(&status);
	printf("Final values: \tshmPtr[0] = %li\tshmPtr[1] = %li\n", shmPtr[0], shmPtr[1]);

	// Detach shared memory segment from address space
	if(shmdt(shmPtr) < 0) {
		perror("just can't let go\n");
		exit(1);
	}
	
	// Destroy the shared memory segment
	if(shmctl(shmId, IPC_RMID, 0) < 0) {
		perror("can't deallocate\n");
		exit(1);
	}

	return 0;
}
