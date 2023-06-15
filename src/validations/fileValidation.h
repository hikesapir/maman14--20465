#ifndef FILE_VALIDATION_H
#define FILE_VALIDATION_H

#include <stdio.h>

/*
This function checks that the file is accessible
And logs an error if it is'nt.

Input:
    char *: a pointer to the file name

Output:
    if the file exists and accessible returns a pointer to the file
    else returns NULL
*/
FILE *validateFile(char *);

#endif