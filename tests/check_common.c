#include <check.h>

#include "../src/common.h"

START_TEST (test_linelen) {
  ck_assert_int_eq(linelen("\n"), 0);
  ck_assert_int_eq(linelen("a\n"), 1);
  ck_assert_int_eq(linelen("abcdefgh\n"), 8);
  ck_assert_int_eq(linelen("abcdefgh"), 8);
  ck_assert_int_eq(linelen("abcdefgh\0ijklmnop"), 8);
  ck_assert_int_eq(linelen("abcdefgh\nijklmnop"), 8);
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
