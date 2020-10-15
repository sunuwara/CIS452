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

void getCountSemMaxValue();
void getShmMaxSize();

int main(int argc, char *argv[]) {
	
	getCountSemMaxValue();
	getShmMaxSize();

	return 0;
}

void getCountSemMaxValue() {

	sem_t countSem;
	int pshared = 0;
	int value = 2147000000;

	// Create an unnamed counting semaphore
	sem_init(&countSem, pshared, value);
	
	// Run until error is returned
	while(sem_post(&countSem) == 0);
	
	sem_getvalue(&countSem, &value);
	printf("Max value of a (counting) semaphore: %d\n", value);
	
	// Destroy counting semaphore
	sem_destroy(&countSem);
}

void getShmMaxSize() {

	int shmId;
	long size = 18796000000;
	
	while(1) {
		size++;
		
		// Create a shared memory segment
		if((shmId = shmget(IPC_PRIVATE, size, IPC_CREAT|S_IRUSR|S_IWUSR)) < 0) {
			printf("Max size of a shared memory segment: %lu\n", size);
			break;
		}
		
		// Destroy the shared memory segment
		shmctl(shmId, IPC_RMID, 0);
	}
	
	
}
