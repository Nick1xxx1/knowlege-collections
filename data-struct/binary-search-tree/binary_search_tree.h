/**
 * @file binary_search_tree.h
 * @author Nick
 * @brief 二叉搜索树实现
 * @version 0.1
 * @date 2023-05-17
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef BINARY_SEARCH_TREE_H_
#define BINARY_SEARCH_TREE_H_

#include <vector>

#include "types.h"

typedef nc_int32_t KEY_TYPE;

#define BSTREE_ENTRY(name, type) \
  struct name {                  \
    struct type *left;           \
    struct type *right;          \
  }

typedef struct BSTreeNode {
  KEY_TYPE key;
  void *value;

  BSTREE_ENTRY(, BSTreeNode) bst;
} BSTreeNode;

typedef struct BSTree {
  struct BSTreeNode *root;

  BSTree() : root(nullptr) {}
} BSTree;

/**
 * @brief 插入键到二叉搜索树中
 * 
 * @param bs_tree 待插入键的二叉搜索树
 * @param key 待插入的键
 * @return Result 插入结果, kOk表示成功, kError表示失败
 */
Result BSTreeInsert(BSTree *bs_tree, KEY_TYPE key);

/**
 * @brief 删除二叉搜索树中指定键的节点
 * 
 * @param bs_tree 待删除值的二叉搜索树
 * @param key 指定需要删除的键
 */
void BSTreeDelete(BSTree *bs_tree, KEY_TYPE key);

/**
 * @brief 查找二叉搜索树中的特定键, 并返回该键对应的节点
 * 
 * @param bs_tree 待查找的二叉搜索树
 * @param key 待查找的键
 * @return BSTreeNode* 查找到的节点, 找不到时返回nullptr
 */
BSTreeNode *BSTreeSearch(BSTree *bs_tree, KEY_TYPE key);

/**
 * @brief 修改二叉搜索树中指定值的节点的值
 * 
 * @param bs_tree 待修改的二叉搜索树
 * @param search_key 需要修改的节点对应的键
 * @param mod_value 修改后的值
 * @return Result 修改结果, kOk表示成功, kError表示失败
 */
Result BSTreeModify(BSTree *bs_tree, KEY_TYPE search_key, void *mod_value);

/**
 * @brief 销毁二叉搜索树
 * 
 * @param bs_tree 待销毁的二叉搜索树
 */
void BSTreeDestroy(BSTree **bs_tree);

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
 * @return std::vector<KEY_TYPE> 存放遍历得到的键
 */
std::vector<KEY_TYPE> BSTreeTraverse(BSTree *bs_tree, TraverseType traverse_type);

#endif // BINARY_SEARCH_TREE_H_
