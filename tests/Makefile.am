AUTOMAKE_OPTIONS = subdir-objects

if TESTS

TESTS_TARGETS = cxkcdpass_test

check_PROGRAMS = $(TESTS_TARGETS)
TESTS = $(TESTS_TARGETS)

cxkcdpass_test_SOURCES = test.c check_random.c check_common.c
cxkcdpass_test_CFLAGS = -Wall @CHECK_CFLAGS@ -I$(top_srcdir)/src
cxkcdpass_test_LDADD = $(top_builddir)/src/random.o $(top_builddir)/src/common.o @CHECK_LIBS@

@VALGRIND_CHECK_RULES@
#VALGRIND_SUPPRESSIONS_FILES = cxkcdpass.supp
#EXTRA_DIST = cxkcdpass.supp

endif
