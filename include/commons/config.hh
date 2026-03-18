#pragma once


namespace cm::LibraryConfig {

/**
 * This causes functions that convert integers to strings to display "(+-)infinity" if the integer value is equal to the
 * min/max possible value of that type.
 */
constexpr inline auto ENABLE_INTEGER_TO_INFINITY = true;

/**
 * This allows functions that convert strings to integers to convert the string "inf" or "infinity" to the respective
 * min/max possible value of that integer type.
 */
constexpr inline auto ENABLE_INTEGER_FROM_INFINITY = true;

/**
 * Operating system page size
 */
constexpr inline auto PAGE_SIZE = 4096uz;

/**
 * Default stack size of worker threads
 */
constexpr inline auto DEFAULT_THREAD_STACK_SIZE = 4'000'000uz;

/**
 * Default stack guard size of worker threads
 */
constexpr inline auto DEFAULT_THREAD_GUARD_SIZE = PAGE_SIZE;

/**
 * Maximum number of pre-initialization assertions
 */
constexpr inline auto MAX_PRE_INIT_ASSERTIONS = 4000;

/**
 * Maximum number of pre-initialization assertion failures are printed (think of it as an "error limit")
 */
constexpr inline auto MAX_PRE_INIT_FAILURES = 10;

constexpr inline auto GLOBAL_CTOR_BASE_PRIO = 110;
constexpr inline auto GLOBAL_CTOR_LIBRARY_INIT_PRIO = GLOBAL_CTOR_BASE_PRIO + 1;
constexpr inline auto GLOBAL_CTOR_ADD_PRECONDITION_PRIO = GLOBAL_CTOR_BASE_PRIO + 3;
constexpr inline auto GLOBAL_CTOR_PRECONDITION_CHECK_PRIO = GLOBAL_CTOR_BASE_PRIO + 999;


constexpr inline auto CONSOLE_ERROR_HIGHLIGHT_BEGIN = "\x1B[31m";
constexpr inline auto CONSOLE_ERROR_HIGHLIGHT_END = "\x1B[0m";

constexpr inline auto MAX_SYMBOL_LENGTH = 192;

constexpr inline auto MAX_STACK_FRAMES = 1024;

}  // namespace cm::LibraryConfig

// ANSI Escape Sequences
namespace cm::AnsiColor {
constexpr inline auto GREEN = "\x1B[32m";
constexpr inline auto RESET = "\x1B[0m";
constexpr inline auto RED = "\x1B[31m";
constexpr inline auto YELLOW = "\x1B[33m";
}
