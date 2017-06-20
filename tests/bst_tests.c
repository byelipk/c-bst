#include "minunit.h"
#include <bst/bst.h>
#include <time.h>

char * test_it_works()
{
    return NULL;
}

char * test_create_destroy()
{
    Tree * tree = Tree_create();
    mu_assert(tree != NULL, "Failed to create tree.");
    mu_assert(tree->data   == NULL, "Should not have data.");
    mu_assert(tree->parent == NULL, "Should not have parent.");
    mu_assert(tree->left   == NULL, "Should not have left child.");
    mu_assert(tree->right  == NULL, "Should not have right child.");

    Tree_destroy(tree);

    return NULL;
}

char * test_searching_empty_tree()
{
    Tree * tree = Tree_create();
    Tree * node = Tree_search(tree, "test1", NULL);

    mu_assert(node == NULL, "Should have been NULL.");

    Tree_destroy(tree);

    return NULL;
}

char * test_inserting_node_into_empty_tree()
{
    bstring test1 = bfromcstr("Nice!");
    Tree * tree   = Tree_create();

    int rc = Tree_insert(tree, &test1, NULL);

    mu_assert(rc == 0, "Failed to insert node in tree.");

    // Cleanup
    Tree_destroy(tree);
    bdestroy(test1);

    return NULL;
}

char * test_searching_for_first_node_in_tree()
{
    bstring test1 = bfromcstr("Nice!");

    Tree * tree = Tree_create();
    int rc = Tree_insert(tree, &test1, NULL);

    mu_assert(rc == 0, "Failed to insert node in tree.");

    Tree * result = Tree_search(tree, &test1, NULL);

    mu_assert(result != NULL, "Failed to find node in tree.");
    mu_assert(result->data == &test1, "Failed to find node in tree.");

    // Cleanup
    Tree_destroy(tree);
    bdestroy(test1);

    return NULL;
}

char * test_inserting_multiple_nodes()
{
    bstring test1 = bfromcstr("Nice!");
    bstring test2 = bfromcstr("Good!");
    bstring test3 = bfromcstr("Sweet!");

    Tree * tree = Tree_create();

    int rc = 0;

    rc = Tree_insert(tree, &test1, NULL);
    mu_assert(rc == 0, "Failed to insert test1 in tree.");
    mu_assert(tree->data == &test1, "Failed to get test1");

    rc = Tree_insert(tree, &test2, NULL);
    mu_assert(rc == 0, "Failed to insert test2 in tree.");
    mu_assert(tree->left->data == &test2, "Failed to get test2");

    rc = Tree_insert(tree, &test3, NULL);
    mu_assert(rc == 0, "Failed to insert test3 in tree.");
    mu_assert(tree->right->data == &test3, "Failed to get test3");

    // Cleanup
    Tree_destroy(tree);

    bdestroy(test1);
    bdestroy(test2);
    bdestroy(test3);

    return NULL;
}

char *all_tests()
{
    mu_suite_start();

    mu_run_test(test_it_works);
    mu_run_test(test_create_destroy);
    mu_run_test(test_searching_empty_tree);
    mu_run_test(test_inserting_node_into_empty_tree);
    mu_run_test(test_searching_for_first_node_in_tree);
    mu_run_test(test_inserting_multiple_nodes);

    return NULL;
}

RUN_TESTS(all_tests);
