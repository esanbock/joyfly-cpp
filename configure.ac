#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([joyfly-cpp], [1.0], [douglas@esanbock.com])
AC_CONFIG_SRCDIR([src/c-joy-test.cc])
AM_INIT_AUTOMAKE
AC_CONFIG_HEADERS([config.h])
AC_REQUIRE_CPP
AC_LANG_CPLUSPLUS

# Checks for programs.
AC_PROG_CXX(g++)

CXXFLAGS="$CXXFLAGS -std=c++11"

# Checks for libraries.
# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL

# Checks for header files.
AC_CHECK_HEADER(SDL2/SDL.h, AC_MSG_RESULT([yes]), AC_MSG_ERROR([SDL2 missing]))
AC_CHECK_HEADER(SerialStream.h, AC_MSG_RESULT([yes]), AC_MSG_ERROR([libserial missing]))

# Checks for library functions.
AC_SEARCH_LIBS([SDL_Init], [SDL2], AC_MSG_RESULT([yes]), AC_MSG_ERROR([SDL2 missing]))
#AC_SEARCH_LIBS([Serial::BAUD_19200], [serial], AC_MSG_RESULT([yes]), AC_MSG_ERROR([libserial missing]) )

AC_OUTPUT([Makefile src/Makefile])
