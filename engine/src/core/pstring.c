#include "core/pstring.h"
#include "core/pmemory.h"

#include <string.h>

u64 str_length(const char *str) {
    return strlen(str);
}

char *str_duplicate(const char *str) {
    u64 length = str_length(str);
    char* copy = palloc(length+1, MEMORY_TAG_STRING);
    pmemcpy(copy, str, length+1);

    return copy;
}
