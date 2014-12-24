#ifndef LIGHTSHIP_UTIL_MEMORY_H
#define LIGHTSHIP_UTIL_MEMORY_H

#include "util/pstdint.h"
#include "util/config.h"

#ifdef MEMORY_ENABLE_MEMORY_REPORT
#   define MALLOC malloc_debug
#   define FREE free_debug
#else
#   define MALLOC malloc
#   define FREE free
#endif

LIGHTSHIP_PUBLIC_API void memory_init(void);
LIGHTSHIP_PUBLIC_API void memory_deinit(void);

#ifdef MEMORY_ENABLE_MEMORY_REPORT
LIGHTSHIP_PUBLIC_API void* malloc_debug(intptr_t size);
LIGHTSHIP_PUBLIC_API void free_debug(void* ptr);
#endif

#endif /* LIGHTSHIP_UTIL_MEMORY_H */
