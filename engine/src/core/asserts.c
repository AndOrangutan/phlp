#include "asserts.h"
#include "logger.h"

void report_assertion_failure(const char* expression, const char* msg, const char* file, i32 line) {
    log_output(LOG_LEVEL_FATAL, "Assertion Failure: %s, message: '%s', in file: %s, line: %d\n", expression, msg, file, line);
}
