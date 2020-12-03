/**************************************************************
 * Title: CIS 452 Lab 10: File System Interface: simulate ls
 * Authors: Aron Sunuwar & Karan Tamang
 * 
 * Description: This is a program that simulates the ls 
 		(directory listing) command. 
 		The simulated ls functions are:
 		- on current directory
 			1. ls		(file names)
	 		2. ls -i	(user and group ids)
	 		3. ls -n	(file inode number)
 		- on different directory with path
			4. ls <directory path>
			5. ls -i <directory path>
			6. ls -n <directory path>
 * ***********************************************************/
 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <time.h>

char *createPermissionsStr(struct stat statBuf);

int main(int argc, char *argv[])
{
    DIR *dirPtr;             // directory pointer
    struct dirent *entryPtr; // entry pointer
    struct stat sb;          // stat buffer
    char timeStr[80];        // timespec to string buffer
    int optionFlag = 0;      // 0 = no option, 1 = "-i" option, 2 = "-n" option
    int dirFlag = 0;         // 0 = current directory, 1 = directory at argv[1]/argv[2]
    int i;                   // argument index integer
    char path[1000];         // path name to directory files

    // Handle argument error and set flags
    if (argc > 3)
    {
        printf("Usage: %s <OPTIONAL: options> <OPTIONAL: directory path>\n", argv[0]);
        exit(1);
    }
    else
    {
        if (argc == 1)
        {
            optionFlag = 0;
            dirFlag = 0;
        }
        else
        {
            if (!strcmp(argv[1], "-i"))
                optionFlag = 1;
            else if (!strcmp(argv[1], "-n"))
                optionFlag = 2;

            if (argc == 3)
                dirFlag = 1;
            else if (argc == 2 && optionFlag == 0)
                dirFlag = 1;
            else
                dirFlag = 0;
        }
    }

    // Open the corresponding directory
    if (dirFlag == 0)
        dirPtr = opendir(".");
    else
    {
        if (optionFlag == 0)
        {
            i = 1;
            dirPtr = opendir(argv[1]);
        }
        else
        {
            i = 2;
            dirPtr = opendir(argv[2]);
        }

        if (stat(argv[i], &sb) < 0)
        {
            perror("stat 1");
            exit(1);
        }

        if (!S_ISDIR(sb.st_mode))
        {
            printf("Usage: invalid directory\n");
            exit(1);
        }
    }

    // Read the directory
    while ((entryPtr = readdir(dirPtr)))
    {
        if (!strcmp(entryPtr->d_name, ".") || !strcmp(entryPtr->d_name, ".."))
            continue;

        if (optionFlag == 0)
            printf("%s\n", entryPtr->d_name);
        else
        {
            // Get info
            if (dirFlag == 0)
            {
                if (stat(entryPtr->d_name, &sb) < 0)
                {
                    perror("stat 2");
                    exit(1);
                }
            }
            else
            {
                sprintf(path, "%s/%s", argv[2], entryPtr->d_name);
                if (lstat(path, &sb) < 0)
                {
                    perror("lstat");
                    exit(1);
                }
            }

            if (optionFlag == 1)
                printf("%ld %8s\n", sb.st_ino, entryPtr->d_name);
            else if (optionFlag == 2)
            {
                strftime(timeStr, 80, "%b %d %H:%M", localtime(&sb.st_ctim.tv_sec));

                printf("%s ", createPermissionsStr(sb));
                printf("%lu ", sb.st_nlink);
                printf("%u ", sb.st_uid);
                printf("%u ", sb.st_gid);
                printf("%5ld ", sb.st_size);
                printf("%5s ", timeStr);
                printf("%5s\n", entryPtr->d_name);
            }
        }
    }

    closedir(dirPtr);

    return 0;
}

char *createPermissionsStr(struct stat statBuf)
{
    static char str[11];

    str[0] = (S_ISDIR(statBuf.st_mode)) ? 'd' : (S_ISLNK(statBuf.st_mode)) ? 'l' : '-';
    str[1] = (statBuf.st_mode & S_IRUSR) ? 'r' : '-';
    str[2] = (statBuf.st_mode & S_IWUSR) ? 'w' : '-';
    str[3] = (statBuf.st_mode & S_IXUSR) ? 'x' : '-';
    str[4] = (statBuf.st_mode & S_IRGRP) ? 'r' : '-';
    str[5] = (statBuf.st_mode & S_IWGRP) ? 'w' : '-';
    str[6] = (statBuf.st_mode & S_IXGRP) ? 'x' : '-';
    str[7] = (statBuf.st_mode & S_IROTH) ? 'r' : '-';
    str[8] = (statBuf.st_mode & S_IWOTH) ? 'w' : '-';
    str[9] = (statBuf.st_mode & S_IXOTH) ? 'x' : '-';
    str[10] = '\0';

    return str;
}

