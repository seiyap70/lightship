#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <wchar.h>
#include <assert.h>
#include "util/log.h"
#include "util/memory.h"
#include "util/string.h"

/* ----------------------------------------------------------------------------
 * Static functions
 * ------------------------------------------------------------------------- */
static int
safe_strlen(const char* str)
{
    if(str)
        return strlen(str);
    return 0;
}

/* ------------------------------------------------------------------------- */
static void
safe_strcat(char* target, const char* source)
{
    assert(target);
    if(source)
        strcat(target, source);
}

/* ------------------------------------------------------------------------- */
static void
safe_strcpy(char* target, const char* source)
{
    assert(target);
    if(source)
        strcpy(target, source);
}

/* ------------------------------------------------------------------------- */
static int
safe_wcslen(const wchar_t* wcs)
{
    if(wcs)
        return wcslen(wcs);
    return 0;
}

/* ------------------------------------------------------------------------- */
static void
safe_wcscat(wchar_t* target, const wchar_t* source)
{
    assert(target);
    if(source)
        wcscat(target, source);
}

/* ------------------------------------------------------------------------- */
static void
safe_wcscpy(wchar_t* target, const wchar_t* source)
{
    assert(target);
    if(source)
        wcscpy(target, source);
}

/* ----------------------------------------------------------------------------
 * Exported functions
 * ------------------------------------------------------------------------- */
void
free_string(void* ptr)
{
    assert(ptr);
    FREE(ptr);
}

/* ------------------------------------------------------------------------- */
void
stdout_strings(uint32_t num_strs, ...)
{
    uint32_t total_length = 0;
    uint32_t i;
    char* buffer;
    va_list ap;
    
    assert(num_strs);

    /* compute total length of all strings combined and allocate a buffer able
     * to contain all strings plus a null terminator */
    va_start(ap, num_strs);
    for(i = 0; i != num_strs; ++i)
        total_length += safe_strlen(va_arg(ap, char*));
    va_end(ap);

    
    buffer = (char*)MALLOC((total_length+1) * sizeof(char));
    if(!buffer)
        OUT_OF_MEMORY("stdout_strings()", RETURN_NOTHING);
    
    /* concatenate all strings into the allocated buffer */
    va_start(ap, num_strs);
    safe_strcpy(buffer, va_arg(ap, char*));
    for(i = 1; i < num_strs; ++i)
        safe_strcat(buffer, va_arg(ap, char*));
    va_end(ap);

    /* print to stdout and clean up */
    fprintf(stdout, "%s\n", buffer);
    FREE(buffer);
}

/* ------------------------------------------------------------------------- */
void
stderr_strings(uint32_t num_strs, ...)
{
    uint32_t total_length = 0;
    uint32_t i;
    char* buffer;
    va_list ap;
    
    assert(num_strs);
    
    /* compute total length of all strings combined and allocate a buffer able
     * to contain all strings plus a null terminator */
    va_start(ap, num_strs);
    for(i = 0; i != num_strs; ++i)
        total_length += safe_strlen(va_arg(ap, char*));
    va_end(ap);
    
    buffer = (char*)MALLOC((total_length+1) * sizeof(char));
    if(!buffer)
        OUT_OF_MEMORY("stderr_strings()", RETURN_NOTHING);
    
    /* concatenate all strings into the allocated buffer */
    va_start(ap, num_strs);
    safe_strcpy(buffer, va_arg(ap, char*));
    for(i = 1; i < num_strs; ++i)
        safe_strcat(buffer, va_arg(ap, char*));
    va_end(ap);

    /* print to stderr and clean up */
    fprintf(stderr, "%s\n", buffer);
    FREE(buffer);
}

/* ------------------------------------------------------------------------- */
char*
cat_strings(uint32_t num_strs, ...)
{
    uint32_t total_length = 0;
    uint32_t i;
    char* buffer;
    va_list ap;
    
    assert(num_strs);

    /* compute total length of all strings combined and allocate a buffer able
     * to contain all strings plus a null terminator */
    va_start(ap, num_strs);
    for(i = 0; i != num_strs; ++i)
        total_length += safe_strlen(va_arg(ap, char*));
    va_end(ap);
    
    buffer = (char*)MALLOC((total_length+1) * sizeof(char));
    if(!buffer)
        OUT_OF_MEMORY("cat_strings()", NULL);
    
    /* concatenate all strings into the allocated buffer */
    va_start(ap, num_strs);
    safe_strcpy(buffer, va_arg(ap, char*));
    for(i = 1; i < num_strs; ++i)
        safe_strcat(buffer, va_arg(ap, char*));
    va_end(ap);
    
    return buffer;
}

/* ------------------------------------------------------------------------- */
char*
malloc_string(const char* str)
{
    char* buffer;
    assert(str);

    buffer = (char*)MALLOC((strlen(str)+1) * sizeof(char));
    if(!buffer)
        OUT_OF_MEMORY("malloc_string()", NULL);
    
    strcpy(buffer, str);
    return buffer;
}

/* ------------------------------------------------------------------------- */
wchar_t*
cat_wstrings(uint32_t num_strs, ...)
{
    uint32_t total_length = 0;
    uint32_t i;
    wchar_t* buffer;
    va_list ap;
    
    assert(num_strs);
    
    /* compute total lenght of all strings combined and allocate a buffer able
     * to contain all strings plus a null terminator */
    va_start(ap, num_strs);
    for(i = 0; i != num_strs; ++i)
        total_length += safe_wcslen(va_arg(ap, wchar_t*));
    va_end(ap);
    
    buffer = (wchar_t*)MALLOC((total_length+1) * sizeof(wchar_t));
    if(!buffer)
        OUT_OF_MEMORY("cat_wstrings()", NULL);
    
    /* concatenate all strings into the allocated buffer */
    va_start(ap, num_strs);
    safe_wcscpy(buffer, va_arg(ap, wchar_t*));
    for(i = 1; i < num_strs; ++i)
        safe_wcscat(buffer, va_arg(ap, wchar_t*));
    va_end(ap);
    
    return buffer;
}

/* ------------------------------------------------------------------------- */
wchar_t*
malloc_wstring(const wchar_t* wcs)
{
    wchar_t* buffer;
    assert(wcs);

    buffer = (wchar_t*)MALLOC((wcslen(wcs)+1) * sizeof(wchar_t));
    if(!buffer)
        OUT_OF_MEMORY("malloc_wstring()", NULL);

    wcscpy(buffer, wcs);
    return buffer;
}

/* ------------------------------------------------------------------------- */
char
is_number(const char chr)
{
    char* numbers = "0123456789";
    while(*numbers)
        if(chr == *numbers++)
            return 1;
    return 0;
}

/* ------------------------------------------------------------------------- */
wchar_t*
strtowcs(const char* str)
{
    wchar_t* wcs;
    wchar_t* wcs_it;
    uint32_t len;
    
    assert(str);
    
    len = strlen(str);

    wcs = (wchar_t*)MALLOC((len + 1) * sizeof(wchar_t));
    if(!wcs)
        OUT_OF_MEMORY("strtowcs()", NULL);

    for(wcs_it = wcs; *str; ++str)
        *wcs_it++ = (wchar_t)*str;
    *wcs_it = L'\0';
    return wcs;
}

/* ------------------------------------------------------------------------- */
char*
wcstostr(wchar_t* wcs)
{
    char* str;
    char* str_it;
    uint32_t len;
    
    assert(wcs);
    
    len = wcslen(wcs);

    str = (char*)MALLOC((len + 1) * sizeof(char));
    if(!str)
        OUT_OF_MEMORY("wcstostr()", NULL);

    for(str_it = str; *wcs; ++wcs)
        *str_it++ = (char)*wcs;
    *str_it = '\0';
    return str;
}

/* ------------------------------------------------------------------------- */
void
crlf2lf(char* src)
{
    char* target = src;
    while(*src)
    {
        if(*src == '\r') /* skip any CRs */
            ++src;
        *target++ = *src++;
    }
    
    /* if at least one CR was skipped, a new null-terminator must be set. */
    if(target != src)
        *target = '\0';
}
