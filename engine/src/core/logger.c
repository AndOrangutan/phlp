#include "logger.h"
#include "platform/platform.h" 

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

    b8 is_error = level < LOG_LEVEL_WARN;

    const i32 msg_length = 32000;

    char in_msg[msg_length];
    memset(in_msg, 0, sizeof(in_msg));

    // To get around MS headers overide
    __builtin_va_list arg_ptr;
    va_start(arg_ptr, msg);
    vsnprintf(in_msg, msg_length, msg, arg_ptr);
    va_end(arg_ptr);

    char out_msg[msg_length];
    sprintf(out_msg, "%s%s\n", level_strings[level], in_msg);

    if (is_error) {
        plat_console_write_error(out_msg, level);
    } else {
        plat_console_write(out_msg, level);
    }

}
