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
void iOption(DIR *dirPtr);
char *createPermissionsStr(struct stat statBuf);

int main(int argc, char *argv[])
{
    DIR *dirPtr;
    //struct dirent *ep; // entry pointer

    if (argc > 3)
    {
        printf("Usage: too many arguments\n");
        exit(1);
    }

    // list current directory contents or argv[2] directory contents
    if (argc <= 2)
        dirPtr = opendir(".");
    else
    {
        dirPtr = opendir(argv[2]);
        printf("one: %s\n", argv[2]);
        
        
    }

    // if there is no argv[1] then just print out name of each file
    // if argv[1] is "-n" then give user and group ID for each file (with other info)
    // if argv[1] is "-i" then give inode # for each file
    if (argc == 1)
        emptyArgvOne(dirPtr);
    else if (!strcmp(argv[1], "-n"))
        nOption(dirPtr);
    else if (!strcmp(argv[1], "-i"))
        iOption(dirPtr);
    else
    {
        // invalid argument
        printf("Usage: invalid argument\n");
        exit(1);
    }

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

        strftime(timeBuffer, 80, "%b %d %H:%M", localtime(&sb.st_ctim.tv_sec));

        printf("%s ", createPermissionsStr(sb));
        printf("%lu ", sb.st_nlink);
        printf("%u ", sb.st_uid);
        printf("%u ", sb.st_gid);
        printf("%5ld ", sb.st_size);
        printf("%5s ", timeBuffer);
        printf("%5s\n", ep->d_name);
    }
}

void iOption(DIR *dirPtr)
{
    struct dirent *ep; // entry pointer
    struct stat sb;    // stat buffer

    while ((ep = readdir(dirPtr)))
    {
        if (stat(ep->d_name, &sb) < 0)
        {
            perror("unsuccessful stat");
            exit(1);
        }

        if (!strcmp(ep->d_name, ".") || !strcmp(ep->d_name, ".."))
            continue;

        printf("%ld %8s\n", sb.st_ino, ep->d_name);
    }
}

char *createPermissionsStr(struct stat statBuf)
{
    static char str[11];

    str[0] = (S_ISDIR(statBuf.st_mode)) ? 'd' : '-';
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

