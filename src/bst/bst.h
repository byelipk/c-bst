#ifndef _bst
#define _bst

#include <stdlib.h>
#include <assert.h>
#include <bstmap/dbg.h>
#include <bstmap/bstrlib.h>
#include <bstmap/bstraux.h>

// The basic operation supported by this recursive structure are
// searching, traversing, insertion, and deletion.
typedef struct Tree {
  void * data;
  struct Tree * parent;
  struct Tree * left;
  struct Tree * right;
} Tree;


typedef int (*Tree_compare)(void * a, void * b);

// Build a new, empty tree
Tree * Tree_create();

// Build a new node to insert into the tree
Tree * Tree_node_create(Tree * parent, void * value);

// Free the tree and all its descendants from memory
void Tree_destroy(Tree * tree);

// Search the tree
// O(h) where h is the height of the tree
Tree * Tree_search(Tree * tree, void * value, Tree_compare compare);

Tree * Tree_find_min(Tree * tree);
Tree * Tree_find_max(Tree * tree);

typedef int (*Traversal_cb)(Tree * tree);

void Tree_traverse_pre_order(Tree * node, Traversal_cb callback);
void Tree_traverse_in_order(Tree * node, Traversal_cb callback);
void Tree_traverse_post_order(Tree * node, Traversal_cb callback);

int Tree_insert(Tree * parent, void * value, Tree_compare compare);


static inline int Tree_root_is_empty(Tree * node) {
  return node->parent == NULL &&
         node->data   == NULL;
}

#endif
