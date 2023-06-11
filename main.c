#include <stdio.h>
#include <stdlib.h>

/* check that the file is accessible */
void validateFile(char *);

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        printf("No filename entered :(\n");
        exit(-1);
    }

    for (int i = 1; i < argc; ++i)
        validateFile(argv[i]);

    return 1;
}

void validateFile(char *fileName)
{
    printf("oppening %s\n", fileName);
    FILE *out_file = fopen(fileName, "w"); /* write only */

    /* test for file not existing. */
    if (out_file == NULL)
    {
        printf("Error! Could not open file\n");
        exit(-1);
    }
    else
    {
        printf("File opened successfully!\n");
        fclose(out_file);
    }
}
