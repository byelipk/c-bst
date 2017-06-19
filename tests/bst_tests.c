#include "minunit.h"
#include <bst/bst.h>

char * test_it_works() {
  return NULL;
}

char * all_tests() {
  mu_suite_start();

  mu_run_test(test_it_works);

  return NULL;
}

RUN_TESTS(all_tests);
