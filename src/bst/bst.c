#include <bst/bst.h>

Tree * Tree_create()
{
    Tree * tree = calloc(1, sizeof(struct Tree));
    check_mem(tree);

    return tree;

error:
    if (tree) {
        Tree_destroy(tree);
    }
    return NULL;
}

static int Tree_destroy_cb(Tree * node)
{
    free(node);
    return 0;
}

void Tree_destroy(Tree * tree)
{
    check(tree != NULL, "Invalid tree.");

    Tree_traverse_in_order(tree, Tree_destroy_cb);

error:
    return;
}

static int default_compare(void * a, void * b)
{
    return bstrcmp((bstring) a, (bstring) b);
}

Tree * Tree_search(Tree * tree, void * value, Tree_compare compare)
{
    check(tree != NULL, "Invalid tree.");

    if (compare == NULL) {
        compare = default_compare; // use the default comparison function
    }

    int cmp = compare(value, tree->data);

    if (cmp == 0) {
        return tree; // The base case
    }

    else if (cmp < 0) {
        return Tree_search(tree->left, value, compare);  // go left
    }

    else {
        return Tree_search(tree->right, value, compare); // go right
    }

error:
    return NULL;
}

void Tree_traverse_pre_order(Tree * node, Traversal_cb callback)
{
    if (node) {
        callback(node->data);

        Tree_traverse_pre_order(node->left, callback);

        Tree_traverse_pre_order(node->right, callback);
    }
}

void Tree_traverse_in_order(Tree * node, Traversal_cb callback)
{
    if (node) {
        Tree_traverse_in_order(node->left, callback);

        callback(node);

        Tree_traverse_in_order(node->right, callback);
    }
}

void Tree_traverse_post_order(Tree * node, Traversal_cb callback)
{
    if (node) {
        Tree_traverse_post_order(node->left, callback);

        Tree_traverse_post_order(node->right, callback);

        callback(node);
    }
}

Tree * Tree_node_create(Tree * parent, void * value)
{
    check(parent != NULL, "Invalid parent node.");

    Tree * node = calloc(1, sizeof(Tree));
    check_mem(node);

    node->data   = value;
    node->parent = parent;
    node->left   = NULL;
    node->right  = NULL;

    return node;

error:
    return NULL;
}

static inline void Tree_insert_node(
    Tree * node, void * value, Tree_compare compare)
{
    check(node != NULL, "Invalid node.");

    if (compare == NULL) {
        compare = default_compare; // use the default comparison function
    }

    int cmp = compare(value, node->data);

    if (cmp <= 0) { // GO LEFT
        if (node->left) {
            Tree_insert_node(node->left, value, compare);
        }
        else {
            node->left = Tree_node_create(node, value);
        }
    }
    else { // GO RIGHT
        if (node->right) {
            Tree_insert_node(node->right, value, compare);
        }
        else {
            node->right = Tree_node_create(node, value);
        }
    }

error:
    return;
}

int Tree_insert(Tree * parent, void * value, Tree_compare compare)
{
    if (Tree_root_is_empty(parent)) {
        parent->data = value;
    }
    else {
        Tree_insert_node(parent, value, compare);
    }

    return 0;
}

Tree * Tree_find_min(Tree * node) {
    check(node != NULL, "Invalid tree.");

    while (node->left) {
        node = node->left;
    }

    return node;

error:
    return NULL;
}

Tree * Tree_find_max(Tree * node) {
    check(node != NULL, "Invalid tree.");

    while (node->right) {
        node = node->right;
    }

    return node;

error:
    return NULL;
}
