AC_PREREQ([2.69])
AC_INIT([Config-Manager], [0.0.1])

AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])

AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([foreign -Wall -Werror])

AX_CHECK_ENABLE_DEBUG()
AS_IF([test "x$enable_debug" = "xno"],
    [CXXFLAGS="${CXXFLAGS} -O3"])

# Compilateurs
AC_PROG_CXX
AC_PROG_CC
AX_CXX_COMPILE_STDCXX([17], [], [mandatory])

# Libtool
AM_PROG_AR
LT_INIT

AX_CHECK_LINK_FLAG(
    [-Wl,--gc-sections],
    [AX_CHECK_COMPILE_FLAG([-ffunction-sections], [CXXFLAGS="${CXXFLAGS} -ffunction-sections"])
     AX_CHECK_COMPILE_FLAG([-fdata-sections], [CXXFLAGS="${CXXFLAGS} -fdata-sections"])
     LDFLAGS="${LDFLAGS} -Wl,--gc-sections"])

AC_ARG_ENABLE(lto,
    [AS_HELP_STRING([--disable-lto=], [enable link-time optimizer])],
    [enable_lto=$enableval],
    [enable_lto=yes])
AS_IF([test "x$enable_lto" = "xyes" -a "x$enable_debug" = "xyes"],
    [AC_MSG_WARN([lto disabled due to debug enabled])
     enable_lto=no])
AS_IF([test "x$enable_lto" = "xyes" ],
    [AX_CHECK_LINK_FLAG(
        [-flto],
        [CXXFLAGS="${CXXFLAGS} -flto"; LDFLAGS="${LDFLAGS} -flto"],
        [AC_MSG_WARN([lto disabled, not supported])])])

AX_CODE_COVERAGE
AM_PATH_PYTHON([3.6])

AC_CONFIG_FILES([
        Makefile
        ext/Makefile
        src/Makefile
        tests/Makefile
])

AC_OUTPUT

AS_IF([test "x$PYTHON" = "x"],
      [AC_MSG_WARN([Python is required to run the tests])])
