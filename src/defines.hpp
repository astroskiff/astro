#ifndef ASTRO_DEFINES_HPP
#define ASTRO_DEFINES_HPP

#define DEBUG_OUTPUT 1

// Remove the leading path from the file name
#define APP_FILE_NAME                                                          \
  (__builtin_strrchr(__FILE__, '/') ? __builtin_strrchr(__FILE__, '/') + 1     \
                                    : __FILE__)

#define APP_LINE __LINE__

// Ensure we have __func__
#if defined(WIN32)
#define __func__ __FUNCTION__
#endif

// Term colors
#if defined(WIN32)

#define TERM_COLOR_BLACK ""
#define TERM_COLOR_RED ""
#define TERM_COLOR_GREEN ""
#define TERM_COLOR_YELLOW ""
#define TERM_COLOR_BLUE ""
#define TERM_COLOR_MAGENTA ""
#define TERM_COLOR_CYAN ""
#define TERM_COLOR_WHITE ""

#define TERM_COLOR_BRIGHT_BLACK ""
#define TERM_COLOR_BRIGHT_RED ""
#define TERM_COLOR_BRIGHT_GREEN ""
#define TERM_COLOR_BRIGHT_YELLOW ""
#define TERM_COLOR_BRIGHT_BLUE ""
#define TERM_COLOR_BRIGHT_MAGENTA ""
#define TERM_COLOR_BRIGHT_CYAN ""
#define TERM_COLOR_BRIGHT_WHITE ""

#define TERM_COLOR_END ""

#else

#define TERM_COLOR_BLACK "\033[0;30m"
#define TERM_COLOR_RED "\033[0;31m"
#define TERM_COLOR_GREEN "\033[0;32m"
#define TERM_COLOR_YELLOW "\033[0;33m"
#define TERM_COLOR_BLUE "\033[0;34m"
#define TERM_COLOR_MAGENTA "\033[0;35m"
#define TERM_COLOR_CYAN "\033[0;36m"
#define TERM_COLOR_WHITE "\033[0;37m"

#define TERM_COLOR_BRIGHT_BLACK "\033[0;90m"
#define TERM_COLOR_BRIGHT_RED "\033[0;91m"
#define TERM_COLOR_BRIGHT_GREEN "\033[0;92m"
#define TERM_COLOR_BRIGHT_YELLOW "\033[0;93m"
#define TERM_COLOR_BRIGHT_BLUE "\033[0;94m"
#define TERM_COLOR_BRIGHT_MAGENTA "\033[0;95m"
#define TERM_COLOR_BRIGHT_CYAN "\033[0;96m"
#define TERM_COLOR_BRIGHT_WHITE "\033[0;97m"

#define TERM_COLOR_END "\033[0m"

#endif

#endif