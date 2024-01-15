#pragma once

#include "defines.h"

#define PASSERTIONS_ENABLED 1

#ifdef PASSERTIONS_ENABLED
#if _MSC_VER
#include <intrin.h>
#define debugBreak() __debugbreak()
#else
#define debugBreak() __builtin_trap()
#endif

/**
 * @brief A wrapper around logger for assertions
 *
 * @param expression Assertion expression
 * @param msg Assertion message
 * @param file Assertion file locaiton
 * @param line Assertion line number
 */
PAPI void report_assertion_failure(const char* expression, const char* msg, const char* file, i32 line);

#define PASSERT(expr) { if (expr) {} else { report_assertion_failure(#expr, "", __FILE__, __LINE__); debugBreak(); } }
#define PASSERT_MSG(expr, msg) { if (expr) {} else { report_assertion_failure(#expr, msg __FILE__, __LINE__); debugBreak(); } }

#ifdef _DEBUG
#define PASSERT_DEBUG(expr) PASSERT(expr)

#else
#define PASSERT_DEBUG(expr)     // Do nothing
#endif

#else

#define PASSERT(expr)           // Do nothing
#define PASSERT_MSG(expr, msg)  // Do nothing
#define KASSERT_DEBUG(expr)     // Do nothing

#endif
