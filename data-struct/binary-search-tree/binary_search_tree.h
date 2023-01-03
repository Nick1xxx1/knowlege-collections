#ifndef _BINARY_SEARCH_TREE_H_
#define _BINARY_SEARCH_TREE_H_

#include <vector>

#include "types.h"

typedef nc_int32_t VALUE_TYPE;

#define BSTREE_ENTRY(name, type) \
  struct name {                  \
    struct type *left;           \
    struct type *right;          \
  }

typedef struct BSTreeNode {
  VALUE_TYPE data;
  BSTREE_ENTRY(, BSTreeNode) bst;
} BSTreeNode;

typedef struct BSTree {
  struct BSTreeNode *root;
} BSTree;

/**
 * @brief 插入值到二叉搜索树中
 * 
 * @param bs_tree 待插入值的二叉搜索树
 * @param value 待插入的值
 * @return nc_int8_t 插入结果, kOk表示成功, kError表示失败
 */
nc_int8_t bstree_insert(BSTree *bs_tree, VALUE_TYPE value);

/**
 * @brief 销毁二叉搜索树
 * 
 * @param bs_tree 待销毁的二叉搜索树
 */
void bstree_destory(BSTree **bs_tree);

enum class TraverseType {
  kPrev = 0,
  kMid,
  kTail,
  kLevel
};

/**
 * @brief 二叉搜索树遍历, 可以指定遍历的方式
 * 
 * @param bs_tree 待遍历的二叉搜索树
 * @param traverse_type 遍历的方式, 包括前中后三种方式
 * @return std::vector<VALUE_TYPE> 存放遍历得到的值
 */
std::vector<VALUE_TYPE> bstree_traverse(BSTree *bs_tree, TraverseType traverse_type);

#endif // _BINARY_SEARCH_TREE_H_
