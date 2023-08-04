#include <stdio.h>
#include <sys/stat.h>
#include <libgen.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "macro.h"
#include "../utils/utils.h"
#include "../logger/logger.h"

FILE *createStrippedFile(char *);
Macros stripMacrosFromSource(FILE *, FILE *);
void replaceMacrosWithContent(FILE *, Macros);
char *getMacroName(char *);
void freeMacros(Macros);

FILE *stripMacros(FILE *sourceFile, char *filePath)
{
    /* Create the new stripped file */
    FILE *stripped_file = createStrippedFile(filePath);

    /* get all macros before stripping the source file */
    Macros macros = stripMacrosFromSource(sourceFile, stripped_file);

    /* closing the source file (end of usage) */
    fclose(sourceFile);

    /* replace every macro instance with the corresponding lines */
    if (macros.amountOfMacros > 0)
        replaceMacrosWithContent(stripped_file, macros);

    /* free macros resources (end of usage) */
    freeMacros(macros);

    return stripped_file;
}

FILE *createStrippedFile(char *filePath)
{
    FILE *stripped_file;
    char fullFileName[PATH_MAX];

    createDirIfNotExists(OUTPUT_FOLDER);

    strcpy(fullFileName, OUTPUT_FOLDER);      /* Add output to the fullFileName*/
    strcat(fullFileName, "\\");               /* Add \ to the fullFileName*/
    strcat(fullFileName, basename(filePath)); /* Copy fileName into fullFileName */
    strcat(fullFileName, ".am");              /* Add the .am suffix to fullFileName */

    logInfo("oppening %s", fullFileName);
    stripped_file = fopen(fullFileName, "w+"); /* read and write */

    if (stripped_file == NULL)
        logError("Could not open file %s", fullFileName);
    else
        logInfo("File opened successfully!");

    return stripped_file;
}

Macros stripMacrosFromSource(FILE *sourceFile, FILE *strippedFile)
{
    Macros macros;
    Macro *newMacro;
    char line[LINE_LENGTH];

    /* initiate the macros array */
    macros.array = (Macro **)malloc(sizeof(Macro *));
    /* initiate the macros count */
    macros.amountOfMacros = 0;

    while (fgets(line, LINE_LENGTH, sourceFile))
    {
        /* if the line contains with "mcro" */
        if (strstr(line, "mcro ") != NULL)
        {
            /* create new Macro */
            macros.amountOfMacros += 1;
            macros.array = (Macro **)realloc(macros.array, macros.amountOfMacros * sizeof(Macro *));
            macros.array[macros.amountOfMacros - 1] = malloc(sizeof(Macro));
            newMacro = macros.array[macros.amountOfMacros - 1];

            /* set the macro's name */
            newMacro->name = getMacroName(line);
            /* set the macro's content */
            newMacro->content = (char **)malloc(sizeof(char *));
            /* set the macro's LinesCount */
            newMacro->amountOfLines = 0;

            /* while we didn't encounter the end of the macro */
            while (strstr(fgets(line, LINE_LENGTH, sourceFile), "endmcro") == NULL)
            {
                /* create new place for the line */
                newMacro->amountOfLines += 1;
                newMacro->content = (char **)realloc(newMacro->content, newMacro->amountOfLines * sizeof(char *));
                newMacro->content[newMacro->amountOfLines - 1] = malloc(sizeof(line));

                /* save the line into the macro's content */
                strcpy(newMacro->content[newMacro->amountOfLines - 1], line);
            }
        }
        else
            /* Non macro & non comment lines => write to the stripped file */
            if (strstr(line, ";") == NULL)
                fprintf(strippedFile, "%s", line);
    }

    return macros;
}

void replaceMacrosWithContent(FILE *strippedFile, Macros macros)
{
    int macroI, ContentI;
    Macro *currentMacro;
    char line[LINE_LENGTH], tempFileName[] = "output/temp.txt";
    bool lineContainsMacro = false;
    FILE *tempFile;

    /* Open a temporary file for writing */
    logInfo("creating temporary file for macro replacements (%s)", tempFileName);
    tempFile = fopen(tempFileName, "w+");
    if (tempFile == NULL)
    {
        logError("creating temporary file failed (%s)", tempFileName);
        return;
    }

    /* Reset the file pointer to the start of the file */
    rewind(strippedFile);

    /* foreach line in file */
    while (fgets(line, LINE_LENGTH, strippedFile))
    {
        lineContainsMacro = false;

        /* foreach macro in macros */
        for (macroI = 0; macroI < macros.amountOfMacros; macroI++)
        {
            currentMacro = macros.array[macroI];
            /* if the line contains the macro name */
            if (strstr(line, currentMacro->name) != NULL)
            {
                lineContainsMacro = true;

                /* replace macro name with macro lines */
                for (ContentI = 0; ContentI < currentMacro->amountOfLines; ContentI++)
                    fprintf(tempFile, "%s", currentMacro->content[ContentI]);

                /* macro found and replaced - we can break from the macro loop */
                break;
            }
        }

        /* if macro not found in current line we want this line intact in the tempfile */
        if (!lineContainsMacro)
            fprintf(tempFile, "%s", line);
    }

    /* Reset the files pointers */
    rewind(strippedFile);
    rewind(tempFile);

    /* Write the modified content back to the file */
    while (fgets(line, LINE_LENGTH, tempFile))
        fprintf(strippedFile, "%s", line);

    /* Remove temp file */
    fclose(tempFile);
    remove(tempFileName);
}

char *getMacroName(char *source)
{
    char *target, *locationOfMCRO = strstr(source, "mcro ");
    int sourceLength = 0, i = 0, j = 0, charactersToMCRO, macroNameLength;

    /* Strip the newline character "mcro <MACRO NAME>\n" => "mcro <MACRO NAME>" */
    source[strcspn(source, "\n")] = 0;

    sourceLength = strlen(source);

    /* get macro name length */
    charactersToMCRO = locationOfMCRO - source;
    macroNameLength = sourceLength - charactersToMCRO - 5;

    target = malloc((macroNameLength) * sizeof(char));

    /* "mcro <MACRO NAME>" */
    for (i = charactersToMCRO + 5, j = 0; i <= sourceLength; i++, j++)
        target[j] = source[i];

    /* assign NULL at the end of string */
    target[j] = '\0';

    return target;
}

void freeMacros(Macros macros)
{
    /* init macro index and content index */
    int Mi, Ci;

    for (Mi = 0; Mi < macros.amountOfMacros; Mi++)
    {
        free(macros.array[Mi]->name);
        for (Ci = 0; Ci < macros.array[Mi]->amountOfLines; Ci++)
            free(macros.array[Mi]->content[Ci]);
        free(macros.array[Mi]->content);
        free(macros.array[Mi]);
    }
    free(macros.array);
}