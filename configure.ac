#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([joyfly-cpp], [1.0], [douglas@esanbock.com])
AC_CONFIG_SRCDIR([src/c-joy-test.cc])
AM_INIT_AUTOMAKE
AC_CONFIG_HEADERS([config.h])

AC_LANG_CPLUSPLUS

# Checks for programs.
#AC_PROG_CXX
#AC_PROG_CC
AC_PROG_CXX(g++)

# Checks for libraries.
# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL

# Checks for header files.
AC_CHECK_HEADER(SDL/SDL.h)
AC_CHECK_HEADER(SerialStream.h)

# Checks for library functions.
AC_SEARCH_LIBS([SDL_Init], [SDL])
AC_SEARCH_LIBS([Serial::BAUD_19200], [serial])

AC_OUTPUT([Makefile src/Makefile])
