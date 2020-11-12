/**************************************************************
 * Title: CIS 452 Lab 8: Memory Mapping
 * Authors: Aron Sunuwar & Karan Tamang
 * 
 * Description: This is a program that shows a diagram of the 
 		memory map of a sample program.
 * ***********************************************************/
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shared.h"

#define SIZE 16

void func(void);

// Global variables
int a;
int b = 1;

int main()
{
	// Local variable
	int c;
	int d = 1;
	
	// Dynamic memory allocation
	char *e = malloc(SIZE);
	int *f = malloc(SIZE);
	
	// Use shared C library
	printHello();
	
	// Print memory map
	printf("---------------\n");
	printf("|    stack    |\n");
	printf("|int c        | - %p - uninitialized local variable\n", &c);
	printf("|int d = 1    | - %p - initialized local variable\n", &d);	
	func();
	printf("|      :      |\n");
	printf("|      V      |\n");
	printf("---------------\n");
	printf("|    heap     |\n");
	printf("|char *e      | - %p - malloc data\n", &e);
	printf("|int *f       | - %p - malloc data\n", &f);	
	printf("|      :      |\n");
	printf("|      V      |\n");
	printf("---------------\n");
	printf("|    data     |\n");
	printf("|int a        | - %p - uninitialized global variable\n", &a);
	printf("|int b = 1    | - %p - initialized global variable\n", &b);
	printf("|      :      |\n");
	printf("|      V      |\n");
	printf("---------------\n");
	printf("|    text     |\n");		
	printf("|func()       | - %p - start of func()\n", &func);
	printf("|main()       | - %p - start of main()\n", &main);
	printf("|printHello() | - %p - shared C library\n", &printHello);	
	printf("|      :      |\n");
	printf("|      V      |\n");
	printf("---------------\n\n");

	// Deallocate memory
	free(e);
	free(f);
	
	return 0;
}

void func(void)
{
	int funcVar;
	printf("|int funcVar  | - %p - variable inside func()\n", &funcVar);
}
