#include "logger.h"

// NOTE: temp
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

b8 init_logger() {
    // TODO: Create log file
    return TRUE;
}

void shutdown_logger() {
    // TODO: Cleanup logging/write queued entries
}

void log_output(log_level level, const char * msg, ...) {
    const char* level_strings[LOG_LEVELS] = {
        "[FATAL]: ",
        "[ERROR]: ",
        "[WARN]: ",
        "[INFO]:",
        "[DEBUG]: ",
        "[TRACE]: "
    };

    /* b8 is_error = level < 2; */

    char in_msg[32000] = {0};

    // To get around MS headers overide
    __builtin_va_list arg_ptr;
    va_start(arg_ptr, msg);
    vsnprintf(in_msg, 32000, msg, arg_ptr);
    va_end(arg_ptr);

    char out_msg[32000];
    sprintf(out_msg, "%s%s\n", level_strings[level], in_msg);

    // NOTE: temp
    printf("%s", out_msg);

}
