#include "util/backtrace.h"
#include <execinfo.h>

char**
get_backtrace(uint32_t* size )
{
    void* array[BACKTRACE_SIZE];
    char** strings;
    
    *size = backtrace(array, BACKTRACE_SIZE);
    strings = backtrace_symbols(array, *size);
    
    return strings;
}
