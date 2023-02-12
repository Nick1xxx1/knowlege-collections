#ifndef B_TREE_H_
#define B_TREE_H_

#include "types.h"

enum class NodeType : nc_uint8_t {
  kNormal = 0,
  kLeaf
};

constexpr nc_int32_t kOrder = 6;

using KEY_TYPE = nc_int32_t;

typedef struct BTreeNode {
  KEY_TYPE *keys;
  BTreeNode **children;

  NodeType type;
  nc_int32_t key_num;

  BTreeNode() : keys(nullptr), children(nullptr), type(NodeType::kNormal), key_num(0) {}
} BTreeNode;

typedef struct BTree {
  BTreeNode *root;
  nc_int32_t degree; // 度, 值为阶数的二分之一

  BTree() : root(nullptr), degree(0) {}
} BTree;

/**
 * @brief 创建一棵B-树
 * 
 * @return BTree* 创建好的B-树对象
 */
BTree *BTreeCreate();

/**
 * @brief 销毁给定的B-树
 * 
 * @param b_tree 待销毁的B-树
 */
void BTreeDestory(BTree *b_tree);

/**
 * @brief 插入键到B-树中
 * 
 * @param b_tree 待插入键的B-树
 * @param key 键
 * @return Result 插入结果, kOk表示成功, kError表示失败
 */
Result BTreeInsert(BTree *b_tree, KEY_TYPE key);

/**
 * @brief 删除B-树中指定键对应的节点
 * 
 * @param b_tree 待删除节点的B-树
 * @param key 键
 * @return Result 删除结果, kOk表示成功, kError表示失败
 */
Result BTreeDelete(BTree *b_tree, KEY_TYPE key);

// TODO: 增加搜索和更新函数

#endif // B_TREE_H_
