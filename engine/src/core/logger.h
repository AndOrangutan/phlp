#pragma once

#include "../defines.h"

#define LOG_WARN_ENABLED 1
#define LOG_INFO_ENABLED 1
#define LOG_DEBUG_ENABLED 1
#define LOG_TRACE_ENABLED 1

#if PRELEASE == 1
    #define LOG_DEBUG_ENABLED 0
    #define LOG_TRACE_ENABLED 0
#endif

// @brief Number of log levels
#define LOG_LEVELS 6

typedef enum log_level {
    LOG_LEVEL_FATAL = 0,
    LOG_LEVEL_ERROR = 1,
    LOG_LEVEL_WARN = 2,
    LOG_LEVEL_INFO = 3,
    LOG_LEVEL_DEBUG = 4,
    LOG_LEVEL_TRACE = 5,
} log_level;

/**
 * @brief Startup Logger
 */
b8 logger_init();

/**
 * @brief Shutdown and save logger queue
 */
void logger_kill();

/**
 * @brief Add message to logger
 * @param level Log level
 * @param msg Formatted string
 */
PAPI void log_output(log_level level, const char* msg, ...);

#define PFATAL(msg, ...) log_output(LOG_LEVEL_FATAL, msg, ##__VA_ARGS__);

#ifndef PERROR
    #define PERROR(msg, ...) log_output(LOG_LEVEL_ERROR, msg, ##__VA_ARGS__);
#endif

#if LOG_WARN_ENABLED == 1
    #define PWARN(msg, ...) log_output(LOG_LEVEL_WARN, msg, ##__VA_ARGS__);
#else
    #define PWARN(msg, ...)
#endif

#if LOG_INFO_ENABLED == 1
    #define PINFO(msg, ...) log_output(LOG_LEVEL_INFO, msg, ##__VA_ARGS__);
#else
    #define PINFO(msg, ...)
#endif

#if LOG_DEBUG_ENABLED == 1
    #define PDEBUG(msg, ...) log_output(LOG_LEVEL_DEBUG, msg, ##__VA_ARGS__);
#else
    #define PDEBUG(msg, ...)
#endif

#if LOG_TRACE_ENABLED == 1
    #define PTRACE(msg, ...) log_output(LOG_LEVEL_TRACE, msg, ##__VA_ARGS__);
#else
    #define PTRACE(msg, ...)
#endif
