#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([joyfly-cpp], [1.0], [douglas@esanbock.com])
AC_CONFIG_SRCDIR([src/main.cpp])
AM_INIT_AUTOMAKE
AC_CONFIG_HEADERS([config.h])
AC_REQUIRE_CPP
AC_LANG_CPLUSPLUS

# Checks for programs.
AC_PROG_CXX(g++)

CXXFLAGS="$CXXFLAGS -std=c++11"

# Checks for libraries.
# Checks for typedefs, structures, and compiler characteristics.

#boost
AC_CONFIG_MACRO_DIR([m4])
AX_BOOST_BASE([1.54],, [AC_MSG_ERROR([boost required])])
AX_BOOST_SYSTEM
AX_BOOST_ASIO

# Checks for header files.
AC_CHECK_HEADER(SDL2/SDL.h, AC_MSG_RESULT([yes]), AC_MSG_ERROR([SDL2 missing]))

# Checks for library functions.
AC_SEARCH_LIBS([SDL_Init], [SDL2], AC_MSG_RESULT([yes]), AC_MSG_ERROR([SDL2 missing]))

AC_OUTPUT([Makefile src/Makefile])
