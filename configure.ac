AC_PREREQ([2.69])
AC_INIT([Config-Manager], [0.0.1])

AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])

AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([foreign -Wall -Werror])

# Compilateurs
AC_PROG_CXX
AC_PROG_CC
AX_CXX_COMPILE_STDCXX([17], [], [mandatory])

# Libtool
AM_PROG_AR
LT_INIT

AC_CONFIG_FILES([
        Makefile
        ext/Makefile
])

AC_OUTPUT
