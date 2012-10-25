#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([MC++-Server], [0.1], [ZetaetaDaniel@gmail.com])
AM_INIT_AUTOMAKE([ foreign])
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([netinet/in.h stdint.h stdlib.h string.h sys/socket.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_INT8_T
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T
AC_TYPE_SSIZE_T

# Checks for library functions.
AC_CHECK_FUNCS([memset memmove socket strerror])

AC_CONFIG_FILES([Makefile])

unused_but_set_variable='
ifneq ($(CXX), clang++)
	unused_but_set_variable=-Wno-error=unused-but-set-variable
else
	unused_but_set_variable=
endif'
AC_SUBST([unused_but_set_variable])

AC_OUTPUT
