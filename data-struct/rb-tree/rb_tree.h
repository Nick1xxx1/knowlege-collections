#ifndef _RB_TREE_H_
#define _RB_TREE_H_

#include "types.h"

enum class NodeColor {
  kRed = 0,
  kBlack
};

typedef nc_int32_t VALUE_TYPE;

#define RBTREE_ENTRY(name, type) \
  struct name {                  \
    struct type *left;           \
    struct type *right;          \
    struct type *parent;         \
  }

typedef struct RBTreeNode {
  VALUE_TYPE value;
  NodeColor node_color;
  RBTREE_ENTRY(, RBTreeNode) rbt;
} RBTreeNode;

#endif // _RB_TREE_H_
