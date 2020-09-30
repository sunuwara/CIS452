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
#include <string.h>

// Declare functions
void* getFile(void* arg);

/* Main program */
int main() {
	
	return 0;
}

/* Prints the diagnostic message with the name of file accessed */
void* getFile(void* arg) {
	
	return arg;
}
