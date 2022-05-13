AC_INIT([cxkcdpass],[0.0.1],[git@christopherjanzon.com],[],[https://github.com/chrillefkr/cxkcdpass])

AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([foreign])
AC_PROG_CC

#AM_CFLAGS="$AM_CFLAGS -Wall -Werror"

AC_CONFIG_FILES([
 Makefile
 src/Makefile
 doc/Makefile
 data/bash_completion.d/Makefile
 tests/Makefile
])


dnl Enable (re)build of documentation files
AC_ARG_ENABLE([docs], AS_HELP_STRING([--enable-docs], [(re)build documentation files. (Default: no)]), [
  case "${enableval}" in
    yes) docs=true ;;
    no)  docs=false ;;
    *) AC_MSG_ERROR([bad value ${enableval} for --enable-docs]) ;;
  esac
], [docs=false])

dnl If we're to (re)build documentation files, check existence of pandoc & texinfo
AS_IF([test "x$enable_docs" = xyes], [
  AC_CHECK_PROG([PANDOC_CHECK], [pandoc], [yes])
  AS_IF([test x"$PANDOC_CHECK" != x"yes"], [AC_MSG_ERROR([Please install pandoc before configuring to build docs.])])
  AC_CHECK_PROG([MAKEINFO_CHECK], [makeinfo], [yes])
  AS_IF([test x"$MAKEINFO_CHECK" != x"yes"], [AC_MSG_ERROR([Please install makeinfo (texinfo package) before configuring to build docs.])])
  AC_CHECK_PROG([PDFLATEX_CHECK], [pdflatex], [yes])
  AS_IF([test x"$PDFLATEX_CHECK" != x"yes"], [AC_MSG_ERROR([Please install pdflatex (texlive package) before configuring to build docs.])])
], [])

AM_CONDITIONAL([BUILD_DOCS], [test "x$docs" = xtrue])

dnl Create --with-libsodium=prefix argument to specify libsodium path for configure script
AC_ARG_WITH([libsodium],
  [AS_HELP_STRING([--with-libsodium=prefix],
      [try this for a non-standard install prefix of the libsodium library])],
  [LIBSODIUMPATHSET=1],
  [LIBSODIUMPATHSET=0])

dnl If --with-libsodium is set, include and link to it; otherwise just verify -lsodium
AS_IF([test $LIBSODIUMPATHSET = 0], [
  AC_CHECK_LIB([sodium], [randombytes_uniform], , [AC_MSG_ERROR([Missing libsodium. https://github.com/jedisct1/libsodium])])
], [
  AM_CFLAGS="$AM_CFLAGS -I$with_libsodium/include"
  AM_LDFLAGS="$AM_LDFLAGS -L$with_libsodium/lib"
  LIBS="-lsodium $LIBS"
])


dnl Enable gprof profiling build
AC_ARG_ENABLE([gprof], AS_HELP_STRING([--enable-gprof], [compile with -pg flag for gprof. (Default: no)]), [
  AS_IF([test "x$enable_gprof" != xno], [
    AM_CFLAGS="-pg $AM_CFLAGS"
  ])
], [])

dnl Enable tests
AC_ARG_ENABLE([tests], AS_HELP_STRING([--enable-tests], [enable tests. (Default: no)]), [
  AS_CASE(["$enableval"],
    [yes], [tests=true],
    [no], [tests=false],
    [AC_MSG_ERROR([bad value ${enableval} for --enable-tests])])
  AS_IF([test "x$enable_tests" != xno], [
    PKG_CHECK_MODULES([CHECK], [check >= 0.9.10], [], [])
    AS_IF([test "x$pkg_failed" != xno], [
      AC_MSG_ERROR([You need check installed to run tests])
    ])
  ])
], [tests=false])
AM_CONDITIONAL([TESTS], [test "x$tests" = xtrue])

dnl Enable valgrind checks, see m4/ax_valgrind_check.m4
AX_VALGRIND_DFLT([memcheck], [on])
AX_VALGRIND_CHECK

AC_SUBST([CHECK_CFLAGS])
AC_SUBST([CHECK_LIBS])

AC_SUBST([AM_CFLAGS])
AC_SUBST([AM_LDFLAGS])

AC_OUTPUT
