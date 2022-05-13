#include <stdlib.h>
#include <check.h>

extern Suite* create_random_suite();
extern Suite* create_common_suite();

int main(int argc, char** argv) {
    SRunner* runner = srunner_create(create_random_suite());
    srunner_add_suite(runner, create_common_suite());
    // srunner_add_suite(runner, create_random_suite());
    srunner_run_all(runner, CK_VERBOSE);
    int failed = srunner_ntests_failed(runner);
    srunner_free(runner);
    return (failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
