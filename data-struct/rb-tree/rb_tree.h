/**
 * @file rb_tree.h
 * @author Nick
 * @brief 红黑树实现
 * @version 0.1
 * @date 2023-05-17
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef RB_TREE_H_
#define RB_TREE_H_

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

/**
 * @brief 创建一个空的红黑树
 * 
 * @return RBTree* 创建的红黑树
 */
RBTree *RBTreeCreate();

/**
 * @brief 销毁红黑树
 * 
 * @param rb_tree 待销毁的红黑树
 */
void RBTreeDestory(RBTree *rb_tree);

/**
 * @brief 向红黑树中插入新的节点
 * 
 * @param rb_tree 待插入节点的红黑树
 * @param insert_node 插入的节点
 */
void RBTreeInsert(RBTree *rb_tree, RBTreeNode *insert_node);

/**
 * @brief 删除红黑树中的节点
 * 
 * @param rb_tree 待删除节点的红黑树
 * @param replace_node 需要删除的节点, 实际上进行替换的节点
 */
void RBTreeDelete(RBTree *rb_tree, RBTreeNode *replace_node);

/**
 * @brief 根据键查找红黑树中的节点
 * 
 * @param rb_tree 待查找的红黑树
 * @param key 键
 * @return RBTreeNode* 查找到的节点
 */
RBTreeNode *RBTreeSearch(RBTree *rb_tree, KEY_TYPE key);

#endif // RB_TREE_H_
