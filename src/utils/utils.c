#include <sys/stat.h>
#include "utils.h"

void createDirIfNotExists(const char *dirname)
{
    struct stat st;
    if (stat(dirname, &st) == -1)
        mkdir(dirname);
}