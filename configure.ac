AC_INIT([cxkcdpass],[0.0.1],[git@christopherjanzon.com],[],[https://github.com/chrillefkr/cxkcdpass])

AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_PROG_CC

AC_CONFIG_FILES([
 Makefile
 src/Makefile
 data/bash_completion.d/Makefile
])

AC_ARG_ENABLE([docs], AS_HELP_STRING([--disable-docs], [create of documentation files. (Default: no)]), [], [])
AS_IF([test "x$enable_docs" != xno], [AC_CONFIG_FILES([doc/Makefile])])

# Create --with-libsodium=prefix argument to specify libsodium path for configure script
AC_ARG_WITH([libsodium],
  [AS_HELP_STRING([--with-libsodium=prefix],
      [try this for a non-standard install prefix of the libsodium library])],
  [LIBSODIUMPATHSET=1],
  [LIBSODIUMPATHSET=0])

# If --with-libsodium is set, include and link to it; otherwise just verify -lsodium
if test $LIBSODIUMPATHSET = 0 ; then
  AC_CHECK_LIB([sodium], [randombytes_uniform], , [AC_MSG_ERROR([Missing libsodium. https://github.com/jedisct1/libsodium])])
else
  AM_CFLAGS="$AM_CFLAGS -I$with_libsodium/include"
  AM_LDFLAGS="$AM_LDFLAGS -L$with_libsodium/lib"
  LIBS="-lsodium $LIBS"
fi

AC_SUBST([AM_CFLAGS])
AC_SUBST([AM_LDFLAGS])

AC_OUTPUT
