#include <stdio.h>
#include <sys/stat.h>
#include <libgen.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "macro.h"
#include "../utils/utils.h"
#include "../logger/logger.h"

/* Decleration */

/**
 * @brief Strips macros from the source file and writes non-macro lines to the stripped file.
 *
 * This function reads the source file, extracts macros, and writes non-macro lines
 * to the stripped file. It returns the collected macros in a `Macros` structure.
 *
 * @param sourceFile A pointer to the source file containing macros.
 * @param strippedFile A pointer to the stripped output file being generated.
 * @return A `Macros` structure containing the extracted macros.
 */
Macros stripMacrosFromSource(FILE *, FILE *);
/**
 * @brief Replaces macro instances with their content in the stripped file.
 *
 * This function replaces instances of macros with their corresponding content in the
 * stripped output file. It uses a temporary file for processing and then overwrites
 * the original stripped file.
 *
 * @param strippedFile A pointer to the stripped output file.
 * @param macros A `Macros` structure containing the extracted macros.
 */
void replaceMacrosWithContent(FILE *, Macros);
/**
 * @brief Extracts the macro name from a macro declaration line.
 *
 * This function extracts the macro name from a macro declaration line.
 * It removes the "mcro " prefix and returns a newly allocated string.
 *
 * @param source The macro declaration line containing the macro name.
 * @return A pointer to the extracted macro name.
 */
char *getMacroName(char *);
/**
 * @brief Frees memory associated with the `Macros` structure.
 *
 * This function releases memory allocated for the `Macros` structure,
 * including macro names and contents.
 *
 * @param macros A `Macros` structure containing the extracted macros.
 */
void freeMacros(Macros);

/* Implementation */

FILE *stripMacros(FILE *sourceFile, char *filePath)
{
    /* Create the new stripped file */
    FILE *stripped_file = create_output_file(filePath, ".am");
    Macros macros;

    if (stripped_file == NULL)
    {
        logError("Could not create file %s.am", filePath);
        return NULL;
    }
    else
        logInfo("File opened successfully!");

    /* get all macros before stripping the source file */
    macros = stripMacrosFromSource(sourceFile, stripped_file);

    /* closing the source file (end of usage) */
    fclose(sourceFile);

    /* replace every macro instance with the corresponding lines */
    if (macros.amountOfMacros > 0)
        replaceMacrosWithContent(stripped_file, macros);

    /* free macros resources (end of usage) */
    freeMacros(macros);

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
    int macro_index, content_index;

    /* Loop through each macro */
    for (macro_index = 0; macro_index < macros.amountOfMacros; macro_index++)
    {
        /* Free memory allocated for the macro's name */
        free(macros.array[macro_index]->name);

        /* Loop through each line of content in the macro */
        for (content_index = 0; content_index < macros.array[macro_index]->amountOfLines; content_index++)
            /* Free memory allocated for the content line */
            free(macros.array[macro_index]->content[content_index]);

        /* Free memory allocated for the content array */
        free(macros.array[macro_index]->content);

        /* Free memory allocated for the macro structure itself */
        free(macros.array[macro_index]);
    }

    /* Free memory allocated for the array of macros */
    free(macros.array);
}