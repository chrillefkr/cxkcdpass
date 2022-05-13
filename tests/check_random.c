#include <stdlib.h> // EXIT_SUCCES, EXIT_FAILURE, free()
#include <check.h>

#include "../src/random.h"

static void setup_random(void) {
    initialize_random();
}

START_TEST (test_get_random_choices) {
  // ck_assert_int_eq(initialize_random(0), 0);

  { // Ensure zero amount returns array with -1 as only element
    int* choices = get_random_choices(0, 0, 0, 0);
    ck_assert_int_eq(choices[0], -1);
    free(choices);
  }
  { // Ensure ten choices actually return ten choices, all zero
    int* ten_choices = get_random_choices(10, 10, 0, 0);
    int ten_choices_count;
    for (ten_choices_count = 0; ten_choices[ten_choices_count] != -1; ten_choices_count++) {
      ck_assert_int_eq(ten_choices[ten_choices_count], 0);
    }
    ck_assert_int_eq(ten_choices_count, 10);
    ck_assert_int_eq(ten_choices[10], -1);
    free(ten_choices);
  }
  { // Ensure ten choices actually return ten choices, all equal to 23
    int* ten_choices = get_random_choices(10, 10, 23, 23);
    int ten_choices_count;
    for (ten_choices_count = 0; ten_choices[ten_choices_count] != -1; ten_choices_count++) {
      ck_assert_int_eq(ten_choices[ten_choices_count], 23);
    }
    ck_assert_int_eq(ten_choices_count, 10);
    ck_assert_int_eq(ten_choices[10], -1);
    free(ten_choices);
  }

  // Ensure some random generated numbers are within limits
  for (int from = 0; from < 100; from += 1) {
    for (int to = 100; to > 0; to -= 1) {
      int* choices = get_random_choices(1, 1, from, to);
      if (from > to) {
        ck_assert_ptr_null(choices);
        continue;
      }
      int value = choices[0];
      ck_assert_int_eq(choices[1], -1);
      ck_assert_int_ge(value, from);
      ck_assert_int_le(value, to);
      free(choices);
    }
  }
}
END_TEST

static TCase* tcase_get_random_choices() {
    TCase* tcase = tcase_create("get_random_choices()");
    tcase_add_checked_fixture(tcase, setup_random, NULL);
    tcase_add_test(tcase, test_get_random_choices);
    return tcase;
}

Suite * create_random_suite(void) {
    Suite *s = suite_create("random");
    suite_add_tcase(s, tcase_get_random_choices());
    return s;
}
