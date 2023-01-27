#ifndef B_TREE_H_
#define B_TREE_H_

#include "types.h"

enum class NodeType : nc_uint8_t {
  kLeaf = 0,
  kNormal
};

constexpr nc_uint32_t kDegree = 5;

using KEY_TYPE = nc_int32_t;

typedef struct BTreeNode {
  KEY_TYPE *keys;
  BTreeNode **children;

  NodeType type;
  nc_uint32_t key_num;

  BTreeNode() : keys(nullptr), children(nullptr), type(NodeType::kNormal), key_num(0) {}
} BTreeNode;

typedef struct BTree {
  BTreeNode *root;
  nc_uint32_t degree; // 度, 值为阶数的二分之一

  BTree() : root(nullptr), degree(0) {}
} BTree;

void BTreeDestory(BTree *b_tree);

nc_int8_t BTreeInsert(BTree *b_tree, KEY_TYPE key);

#endif // B_TREE_H_
