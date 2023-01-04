#ifndef _RB_TREE_H_
#define _RB_TREE_H_

#include "types.h"

enum class NodeColor : nc_uint8_t {
  kRed = 0,
  kBlack
};

typedef nc_int32_t KEY_TYPE;

#define RBTREE_ENTRY(name, type) \
  struct name {                  \
    struct type *left;           \
    struct type *right;          \
    struct type *parent;         \
  }

typedef struct RBTreeNode {
  KEY_TYPE key;
  void *value;
  NodeColor node_color;
  RBTREE_ENTRY(, RBTreeNode) rbt;
} RBTreeNode;

typedef struct RBTree {
  RBTreeNode *root;
  RBTreeNode *nil;
} RBTree;

#endif // _RB_TREE_H_
