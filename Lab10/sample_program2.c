#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

int main()
{
	DIR *dirPtr;
	struct dirent *entryPtr;
	struct stat statBuf;
	
	dirPtr = opendir (".");
	
	while ((entryPtr = readdir (dirPtr))) {
		if (stat (entryPtr->d_name, &statBuf) < 0) {
			perror ("huh?  there is ");
			exit(1);
		}
		
		printf ("%ld bytes	%-20s\n", statBuf.st_size, entryPtr->d_name);
	}
	
	closedir (dirPtr);
	
	return 0;
}
