#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <time.h>

void emptyArgvOne(DIR *dirPtr);
void nOption(DIR *dirPtr);
char *createPermissionsStr(struct stat statBuf);

int main(int argc, char *argv[])
{
    DIR *dirPtr;
    // struct dirent *entryPtr;
    // struct stat statBuf;

    // arguments need to be <= 3
    if (argc > 3)
    {
        printf("Usage: too many arguments\n");
        exit(1);
    }

    // list current directory contents
    if (argc <= 2)
    {
        dirPtr = opendir(".");

        // if argv[1] is " " then just print out name of each file
        if (argc == 1)
            emptyArgvOne(dirPtr);

        // if argv[1] is -n then give user and group ID for each file (with other info)
        else if (!strcmp(argv[1], "-n"))
            nOption(dirPtr);

        // // if argv[1] is -i then give inode # for each file
        // else if (!strcmp(argv[1], "-i"))
        // {
        // }

        // invalid argument
        else
        {
            printf("Usage: invalid argument\n");
            exit(1);
        }
    }

    // // list argv[2] directory contents
    // else
    // {
    //     dirPtr = opendir(argv[2]);
    //     // if argv[1] is " " then just print out name of each file
    //     if (argc == 1)
    //     {
    //     }

    //     // if argv[1] is -n then give user and group ID for each file (with other info)
    //     else if (strcmp(argv[1], "-n") == 0)
    //     {
    //     }

    //     // if argv[1] is -i then give inode # for each file
    //     else if (strcmp(argv[1], "-i") == 0)
    //     {
    //     }

    //     // invalid argument
    //     else
    //     {
    //         printf("Usage: invalid argument\n");
    //         exit(1);
    //     }
    // }

    closedir(dirPtr);

    return 0;
}

void emptyArgvOne(DIR *dirPtr)
{
    struct dirent *ep;

    while ((ep = readdir(dirPtr)))
    {
        if (!strcmp(ep->d_name, ".") || !strcmp(ep->d_name, ".."))
            continue;
        printf("%s  ", ep->d_name);
    }
    printf("\n");
}

void nOption(DIR *dirPtr)
{
    struct dirent *ep;   // entry pointer
    struct stat sb;      // stat buffer
    char timeBuffer[80]; // timespec to string buffer

    while ((ep = readdir(dirPtr)))
    {
        if (stat(ep->d_name, &sb) < 0)
        {
            perror("unsuccessful stat");
            exit(1);
        }

        if (!strcmp(ep->d_name, ".") || !strcmp(ep->d_name, ".."))
            continue;

	printf(createPermissionsStr(sb));
	
        strftime(timeBuffer, 80, "%b %d %H:%M", localtime(&sb.st_ctim.tv_sec));
        printf("%o %ld %u %u %5ld %5s %5s\n", sb.st_mode, sb.st_nlink, sb.st_uid, sb.st_gid, sb.st_size, timeBuffer, ep->d_name);
    }
}

char *createPermissionsStr(struct stat statBuf) 
{
	char str[10];
	char temp;
	
	strcat(str, (S_ISDIR(statBuf.st_mode)) ? "d" : "-");
	strcat(str, (statBuf.st_mode & S_IRUSR) ? "r" : "-");
	strcat(str, (statBuf.st_mode & S_IWUSR) ? "w" : "-");
	strcat(str, (statBuf.st_mode & S_IXUSR) ? "x" : "-");
	strcat(str, (statBuf.st_mode & S_IRGRP) ? "r" : "-");
	strcat(str, (statBuf.st_mode & S_IWGRP) ? "w" : "-");
	strcat(str, (statBuf.st_mode & S_IXGRP) ? "x" : "-");
	strcat(str, (statBuf.st_mode & S_IROTH) ? "r" : "-");
	strcat(str, (statBuf.st_mode & S_IWOTH) ? "w" : "-");
	strcat(str, (statBuf.st_mode & S_IXOTH) ? "x" : "-");
	
	return str;
}



