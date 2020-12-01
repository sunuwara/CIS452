#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

int main(int argc, char *argv[])
{
	struct stat statBuf;

	if (argc < 2) {
		printf ("Usage: filename required\n");
		exit(1);
	}

	if (stat (argv[1], &statBuf) < 0) {
		perror ("huh?  there is ");
		exit(1);
	}
	
	printf ("value is: %u\n", statBuf.st_mode);
	
	if (S_ISDIR(statBuf.st_mode)) {
		/* Handle directory */
		printf("Input is a directory!\n");
	}
	else {
		printf("Input is not a directory!\n");
	}
	
	return 0;
}
