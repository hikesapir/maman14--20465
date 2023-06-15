#include <string.h>
#include "symbolsTable.h"
#include "util.h"

int main()
{
    makeSymbolsTable("origin.as");
    return 0;
}

int makeSymbolsTable(char *fileName)
{
    FILE *fptr;
    /* Store the content of the file*/
    line myLine;

    int lineIdx;

    /* Open a file in read mode*/
    fptr = fopen(fileName, "r");

    /* If the file exist*/
    if (fptr != NULL)
    {

        /* Read the content and print it */
        while (fgets(myLine, 100, fptr))
        {
            lineIdx = indexOfChar(myLine, LABEL_SIGN);

            if (myLine[0] == GUIDE_SIGN)
            {
                printf("guide CMD");
            }
            printf("%s", myLine);
            printf("has label: %d\n", lineIdx);
        }

        /* If the file does not exist */
    }
    else
    {
        printf("Not able to open the file.");
    }

    /* Close the file */
    fclose(fptr);
    return 0;
}
