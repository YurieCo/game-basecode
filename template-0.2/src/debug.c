#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>

#include "debug.h"

#ifdef _WIN32
static FILE *out = NULL;
#endif

#ifdef _WIN32
static void close(void)
{
    if (out)
        fclose(out);
}
#endif

void retrodebug(char *str, ...)
{
    va_list args;
    va_start(args, str);
#ifdef _WIN32
    if (!out)
    {
        out = fopen("debug.txt", "w");
        atexit(close);
    }
    vfprintf(out, str, args);
#else
    vprintf(str, args);
#endif
}
