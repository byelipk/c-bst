#include <bst/bst.h>

static int default_compare(void *a, void *b)
{
    return bstrcmp((bstring) a, (bstring) b);
}

BSTree *BSTree_create(BSTree_compare compare)
{
    BSTree *map = calloc(1, sizeof(BSTree));
    check_mem(map);

    map->compare = compare == NULL ? default_compare : compare;

    return map;

error:
    if (map) {
        BSTree_destroy(map);
    }
    return NULL;
}

static int BSTree_destroy_cb(BSTreeNode * node)
{
    free(node);
    return 0;
}

void BSTree_destroy(BSTree * map)
{
    if (map) {
        BSTree_traverse(map, BSTree_destroy_cb);
        free(map);
    }
}

static inline BSTreeNode *BSTreeNode_create(
  BSTreeNode * parent, void *key, void *data)
{
    BSTreeNode *node = calloc(1, sizeof(BSTreeNode));
    check_mem(node);

    node->key = key;
    node->data = data;
    node->parent = parent;
    return node;

error:
    return NULL;
}

static inline void BSTree_setnode(
    BSTree * map, BSTreeNode * node, void *key, void *data)
{
    check(map != NULL, "Invalid map.");
    check(node != NULL, "Invalid node.");

    int cmp = map->compare(node->key, key);

    if (cmp <= 0) {
        if (node->left) {
            BSTree_setnode(map, node->left, key, data);
        } else {
            node->left = BSTreeNode_create(node, key, data);
        }
    } else {
        if (node->right) {
            BSTree_setnode(map, node->right, key, data);
        } else {
            node->right = BSTreeNode_create(node, key, data);
        }
    }

error:
    return;
}

int BSTree_set(BSTree * map, void *key, void *data)
{
    check(map != NULL, "Invalid map.");

    if (map->root == NULL) {
        // first so just make it and get out
        map->root = BSTreeNode_create(NULL, key, data);
        check_mem(map->root);
    } else {
        BSTree_setnode(map, map->root, key, data);
    }

    return 0;
error:
    return -1;
}

static inline BSTreeNode *BSTree_getnode(
  BSTree * map, BSTreeNode * node, void *key)
{
    check(map != NULL, "Invalid map.");
    check(node != NULL, "Invalid node.");

    int cmp = map->compare(node->key, key);

    if (cmp == 0) {
        return node;
    } else if (cmp < 0) {
        if (node->left) {
            return BSTree_getnode(map, node->left, key);
        } else {
            return NULL;
        }
    } else {
        if (node->right) {
            return BSTree_getnode(map, node->right, key);
        } else {
            return NULL;
        }
    }

error:
    return NULL;
}

void *BSTree_get(BSTree * map, void *key)
{
    check(map != NULL, "Invalid map.");

    if (map->root == NULL) {
        return NULL;
    } else {
        BSTreeNode *node = BSTree_getnode(map, map->root, key);
        return node == NULL ? NULL : bdata(node);
    }

error:
    return NULL;
}

// Uses post order depth-first traversal
static inline int BSTree_traverse_nodes(
    BSTreeNode * node, BSTree_traverse_cb traverse_cb)
{
    check(node != NULL, "Invalid node.");

    int rc = 0;

    if (node->left) {
        rc = BSTree_traverse_nodes(node->left, traverse_cb);
        if (rc != 0) {
            return rc;
        }
    }

    if (node->right) {
        rc = BSTree_traverse_nodes(node->right, traverse_cb);
        if (rc != 0) {
            return rc;
        }
    }

    return traverse_cb(node);

error:
    return -1;
}

int BSTree_traverse(BSTree * map, BSTree_traverse_cb traverse_cb)
{
    check(map != NULL, "Invalid map.");

    if (map->root) {
        return BSTree_traverse_nodes(map->root, traverse_cb);
    }
    else {
      return 0;
    }

error:
    return -1;
}

static inline BSTreeNode *BSTree_find_min(BSTreeNode * node)
{
    while (node->left) {
        node = node->left;
    }

    return node;
}

static inline void BSTree_replace_node_in_parent(
    BSTree * map, BSTreeNode * node, BSTreeNode * new_value)
{
    check(map != NULL, "Invalid map.");
    check(node != NULL, "Invalid node.");

    if (node->parent) {
        if (node == node->parent->left) {
            node->parent->left = new_value;
        } else {
            node->parent->right = new_value;
        }
    } else {
        // this is the root so gotta change it
        map->root = new_value;
    }

    if (new_value) {
        new_value->parent = node->parent;
    }

error:
    return;
}

static inline void BSTree_swap(BSTreeNode * a, BSTreeNode * b)
{
    void *temp = NULL;
    temp = b->key;
    b->key = a->key;
    a->key = temp;
    temp = b->data;
    b->data = a->data;
    a->data = temp;
}

static inline BSTreeNode * BSTree_node_delete(
    BSTree * map, BSTreeNode * node, void *key)
{
    check(map != NULL, "Invalid map.");
    check(node != NULL, "Invalid node.");

    // Run the comparison function. Result will be 0, 1, or -1
    int cmp = map->compare(node->key, key);

    if (cmp < 0) {
        // Recurse down the left subtree
        if (node->left) {
            return BSTree_node_delete(map, node->left, key);
        }
        // We did not find the key, nothing else to do
        else {
            return NULL;
        }
    }

    else if (cmp > 0) {
        // Recurse down the right subtree
        if (node->right) {
            return BSTree_node_delete(map, node->right, key);
        }
        // We did not find the key, nothing else to do
        else {
            return NULL;
        }
    }

    // We've found the node we want to delete. Now we have to
    // solve for three cases:
    //
    //  1. The node has no children
    //  2. The node has 1 child (left or right)
    //  3. The node has two children
    else {

        // What to do with a node with two children?
        //
        // The solution is to relabel this node with the key of its
        // immediate successor in sorted order. This successor must
        // be the smallest value in the right subtree, specifically
        // the leftmost descendant in the right subtree. Moving this
        // successor to the point of deletion results in a properly
        // labeled binary search tree, and reduces our deletion
        // problem to physically removing a node with at most one
        // child.
        if (node->left && node->right) {

            // Find the leftmost descendant in the right subtree
            BSTreeNode * successor = BSTree_find_min(node->right);

            // Move the successor to the point of deletion
            BSTree_swap(successor, node);

            // Now our problem is reduced to physically removing a node
            // with one child at most.
            BSTree_replace_node_in_parent(map, successor, successor->right);

            // Finally it's swapped, so return successor instead of node
            return successor;
        }

        // One child on the left
        else if (node->left) {
            BSTree_replace_node_in_parent(map, node, node->left);
        }

        // One child on the right
        else if (node->right) {
            BSTree_replace_node_in_parent(map, node, node->right);
        }
        
        // No children
        else {
            BSTree_replace_node_in_parent(map, node, NULL);
        }

        return node;
    }

error:
    return NULL;
}

void *BSTree_delete(BSTree * map, void *key)
{
    check(map != NULL, "Invalid map.");

    void *data = NULL;

    if (map->root) {
        BSTreeNode *node = BSTree_node_delete(map, map->root, key);

        if (node) {
            data = node->data;
            free(node);
        }
    }

    return data;
error:
    return NULL;
}
