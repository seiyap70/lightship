#include "util/backtrace.h"
#include <stdio.h>

char**
get_backtrace(uint32_t* size )
{
    /* TODO */
    fprintf(stderr, "backtraces not implemented on windows\n");
    *size = 0;
    return (char**)0;
}
