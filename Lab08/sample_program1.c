/* REVISED SAMPLE PROGRAM 1 WITH SEGMENTATION FAULT FIXED */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 16

int main()
{
    char *data1;

    data1 = malloc(SIZE);
    printf ("Please input username (will only take %d characters): ", (SIZE - 1));
    
    // Only 15 characters of input will be read
    scanf ("%15s", data1);
    
    printf ("you entered: %s\n", data1);
    free (data1);
    return 0;
}
