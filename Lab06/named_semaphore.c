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
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>

#define SIZE 16

#define SEM_MUTEX_NAME "/sem-mutex"

int main(int argc, char *argv[]) {

	int status;
	long int i, loop, temp, *shmPtr;
	int shmId;
	pid_t pid;
	sem_t *semMutex;

	// Get value of loop variable (from command-line argument)
	if(argc != 2) {
		perror("invalid number of arguments\n");
		exit(1);
	}
	loop = atoi(argv[1]);
	printf("Loop = %ld\n", loop);
	
	// Create a shared memory segment
	if((shmId = shmget(IPC_PRIVATE, SIZE, IPC_CREAT|S_IRUSR|S_IWUSR)) < 0) {
		perror("shmget\n");
		exit(1);
	}
	
	// Attach shared memory segment to address space 
	if((shmPtr = shmat(shmId, 0, 0)) == (void*) -1) {
		perror("shmat\n");
		exit(1);
	}

	// Initial values for shared data
	shmPtr[0] = 0;
	shmPtr[1] = 1;
	printf("Initial values: shmPtr[0] = %li\tshmPtr[1] = %li\n", shmPtr[0], shmPtr[1]);

	// Create a named mutual exclusion semaphore, semMutex with an initial value 1
	if ((semMutex = sem_open(SEM_MUTEX_NAME, O_CREAT, 0660, 1)) == SEM_FAILED) {
		perror ("sem_open\n"); 
		exit (1);
	}
	
	// Fork child, both child and parent conduct own swapping 
	if(!(pid = fork())) {
	
		// Child process
		for(i = 0; i < loop; i++) {
		
			// Semaphore wait
			if((sem_wait(semMutex)) == -1) {
				perror("child sem_wait\n");
				exit(1);
			}
		
			// Critical section --> swap the contents of shmPtr[0] and shmPtr[1]
			temp = shmPtr[0];
			shmPtr[0] = shmPtr[1];
			shmPtr[1] = temp;
			// End Critical section
			
			// Semaphore signal
			if((sem_post(semMutex)) == -1) {
				perror("child sem_post\n");
				exit(1);
			}
		}
		
		// Close childs mutex semaphore
		if((sem_close(semMutex)) == -1) {
			perror("child sem_close\n");
			exit(1);
		}
		
		// Detach shared memory segment from address space
		if(shmdt(shmPtr) < 0) {
			perror("child shmdt\n");
			exit(1);
		}
		
		exit(0);
	}
	else {
		
		// Parent Process
		for(i = 0; i < loop; i++) {
		
			// Semaphore wait
			if((sem_wait(semMutex)) == -1) {
				perror("parent sem_wait\n");
				exit(1);
			}
		
			// Critical section --> swap the contents of shmPtr[0] and shmPtr[1]
			temp = shmPtr[0];
			shmPtr[0] = shmPtr[1];
			shmPtr[1] = temp;
			// End Critical section
			
			// Semaphore signal
			if((sem_post(semMutex)) == -1) {
				perror("parent sem_post\n");
				exit(1);
			}
		}
	}

	// Parent waits for child to finish and print the final output
	wait(&status);
	printf("Final values: \tshmPtr[0] = %li\tshmPtr[1] = %li\n", shmPtr[0], shmPtr[1]);
	
	// Close parents mutex semaphore
	if((sem_close(semMutex)) == -1) {
		perror("parent sem_close\n");
		exit(1);
	}
		
	// Remove mutex semaphore
	if (sem_unlink(SEM_MUTEX_NAME) == -1) {
		perror("sem_unlink\n"); 
		exit (1);
	}
	
	// Detach shared memory segment from address space
	if(shmdt(shmPtr) < 0) {
		perror("parent shmdt\n");
		exit(1);
	}
	
	// Destroy the shared memory segment
	if(shmctl(shmId, IPC_RMID, 0) < 0) {
		perror("deallocate shmctl\n");
		exit(1);
	}

	return 0;
}
