#include "util.h"

/*
returns the position of the first occurrence of a value in a string.
returns -1 if the value is not found
*/
int indexOfChar(char *str, char c)
{
    int i = 0;
    char currChar = str[0];
    while (currChar != '\0')
    {
        if (currChar == c)
            return i;

        i++;
	currChar = str[i];
    }

    return -1;
}
