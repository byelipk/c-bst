#ifndef _bst
#define _bst

#include <stdlib.h>
#include <assert.h>
#include <bst/dbg.h>
#include <bst/bstrlib.h>
#include <bst/bstraux.h>


// Binary Search Tree
//
// The best way to describe this implementation is that it is like
// a Hashmap style key/value store. The difference is that instead
// of hashing the key, the BST compares the key to nodes in a tree,
// and then walks through the tree to find the best place to store
// it, based on how it compares to other nodes.

typedef int (*BSTree_compare)(void * a, void * b);

typedef struct BSTreeNode {
  void * key;
  void * data;

  struct BSTreeNode * parent;
  struct BSTreeNode * left;
  struct BSTreeNode * right;
} BSTreeNode;

typedef struct BSTree {
  int count;
  BSTree_compare compare;
  BSTreeNode * root;
} BSTree;

typedef int (*BSTree_traverse_cb)(BSTreeNode * node);

BSTree * BSTree_create(BSTree_compare compare);
void BSTree_destroy(BSTree * map);

// Algorithm
//
// 1. If there is no root node, then I just make it and we're done.
// 2. If there is a root node, I compare your key with that node's key.
// 3. If your key is less than or equal to that node's key,
//    then I go left. If your key is greater than the node's key,
//    then I go right.
// 4. I keep repeating these steps until I reach a node where left or
//    right doesn't exist.
// 5. Once there, I set that direction (left or right) to a new node
//    for the key and data I want. Then I set this new node's parent
//    to the previous node I came from. I'll use the parent node when
//    I want to delete this new node.
int BSTree_set(BSTree * map, void * key, void * data);

// Algorithm
//
// 1. I take the key you're looking for and I start at the root node.
// 2. I compare your key with that node's key.
// 3. If your key is less than that node's key, I traverse down the
//    tree using the left pointer.
// 4. If your key is greater than that node's key, I traverse down
//    using the right pointer.
// 5. I repeat these steps until I find a node with a matching key,
//    or I get to a node with no left and right pointers.
// 6. If I find a match, I return that node's data. Otherwise I will
//    return NULL.
void * BSTree_get(BSTree * map, void * key);

int BSTree_traverse(BSTree * map, BSTree_traverse_cb traverse_cb);

void * BSTree_delete(BSTree * map, void * key);

#endif
