#include "../symbol/symbol.h"
#include "../command/command.h"

/**
 * @brief Write all output files for commands and symbols.
 *
 * This function coordinates the writing of various output files including symbol entries, external symbols,
 * and base64-encoded commands and static data.
 *
 * @param commands A Commands structure containing command information.
 * @param symbols A Symbols structure containing symbol information.
 * @param file_name The base name of the output file.
 */
void write_files(Commands, Symbols, char *);
