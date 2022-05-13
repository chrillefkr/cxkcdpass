#include <check.h>

#include "../src/common.h"

START_TEST (test_linelen) {
  // mmap_wordfile()
}
END_TEST

static TCase* tcase_linelen() {
    TCase* tcase = tcase_create("linelen()");
    tcase_add_test(tcase, test_linelen);
    return tcase;
}

Suite * create_common_suite(void) {
    Suite *s = suite_create("common");
    suite_add_tcase(s, tcase_linelen());
    return s;
}
