#include "binary_search_tree.h"

#include <deque>
#include <exception>
#include <iostream>
#include <stack>

using std::cerr;
using std::deque;
using std::endl;
using std::stack;
using std::vector;

/**
 * @brief 创建二叉搜索树的节点
 * 
 * @param value 赋予给节点的值
 * @return BSTreeNode* 创建的节点 
 */
static BSTreeNode *bstree_create_node(VALUE_TYPE value);

/**
 * @brief 销毁节点函数
 * 
 * @param bs_tree_node 待销毁的节点
 */
static void bstree_destory_node(BSTreeNode *bs_tree_node);

/**
 * @brief 二叉搜索树前序遍历
 * 
 * @param bs_tree 待遍历的二叉搜索树
 * @return std::vector<VALUE_TYPE> 存放遍历得到的值 
 */
static vector<VALUE_TYPE> bstree_traverse_prev(BSTree *bs_tree);

/**
 * @brief 二叉搜索树中序遍历
 * 
 * @param bs_tree 待遍历的二叉搜索树
 * @return std::vector<VALUE_TYPE> 存放遍历得到的值 
 */
static vector<VALUE_TYPE> bstree_traverse_mid(BSTree *bs_tree);

/**
 * @brief 二叉搜索树后序遍历
 * 
 * @param bs_tree 待遍历的二叉搜索树
 * @return std::vector<VALUE_TYPE> 存放遍历得到的值 
 */
static vector<VALUE_TYPE> bstree_traverse_tail(BSTree *bs_tree);

/**
 * @brief 二叉搜索树层序遍历
 * 
 * @param bs_tree 待遍历的二叉搜索树
 * @return std::vector<VALUE_TYPE> 存放遍历得到的值
 */
static vector<VALUE_TYPE> bstree_traverse_level(BSTree *bs_tree);

static BSTreeNode *bstree_create_node(VALUE_TYPE value) {
  BSTreeNode *node = nullptr;
  try {
    node = new BSTreeNode;
  } catch (std::bad_alloc &ex) {
    return nullptr;
  }

  node->data = value;
  node->bst.left = node->bst.right = nullptr;

  return node;
}

nc_int8_t bstree_insert(BSTree *bs_tree, VALUE_TYPE value) {
  if (!bs_tree) {
    return kError;
  }

  if (!bs_tree->root) {
    bs_tree->root = bstree_create_node(value);
    return kOk;
  }

  BSTreeNode *node = bs_tree->root;
  BSTreeNode *tmp = nullptr;

  while (node) {
    tmp = node;

    if (value < node->data) {
      node = node->bst.left;
    } else if (value > node->data) {
      node = node->bst.right;
    } else {
      cerr << "Duplicate node" << endl;
      return kError;
    }
  }

  if (value < tmp->data) {
    tmp->bst.left = bstree_create_node(value);
  } else {
    tmp->bst.right = bstree_create_node(value);
  }

  return kOk;
}

nc_int8_t bstree_delete(BSTree *bs_tree, VALUE_TYPE value) {
  
  return kOk;
}

BSTreeNode *bstree_search(BSTree *bs_tree, VALUE_TYPE value) {
  if (!bs_tree || !bs_tree->root) {
    return nullptr;
  }

  BSTreeNode *node = bs_tree->root;
  while (node) {
    if (node->data == value) {
      return node;
    }

    if (node->data > value) {
      node = node->bst.left;
    } else {
      node = node->bst.right;
    }
  }

  return nullptr;
}

nc_int8_t bstree_modify(BSTree *bs_tree, VALUE_TYPE search_value, VALUE_TYPE mod_value) {
  return kOk;
}

static void bstree_destory_node(BSTreeNode *bs_tree_node) {
  if (!bs_tree_node) {
    return;
  }

  if (bs_tree_node->bst.left) {
    bstree_destory_node(bs_tree_node->bst.left);
    bs_tree_node->bst.left = nullptr;
  }

  if (bs_tree_node->bst.right) {
    bstree_destory_node(bs_tree_node->bst.right);
    bs_tree_node->bst.right = nullptr;
  }

  if (bs_tree_node) {
    delete bs_tree_node;
    bs_tree_node = nullptr;
  }
}

void bstree_destory(BSTree **bs_tree) {
  if (!*bs_tree) {
    return;
  }

  if (!(*bs_tree)->root) {
    delete bs_tree;
    *bs_tree = nullptr;
    return;
  }

  bstree_destory_node((*bs_tree)->root);

  delete *bs_tree;
  *bs_tree = nullptr;
}

static vector<VALUE_TYPE> bstree_traverse_prev(BSTree *bs_tree) {
  if (!bs_tree || !bs_tree->root) {
    return {};
  }

  stack<BSTreeNode*> stk;
  BSTreeNode *node = bs_tree->root;

  vector<VALUE_TYPE> res;
  while (node || !stk.empty()) {
    if (node) {
      res.push_back(node->data);
      stk.push(node);
      node = node->bst.left;
    } else {
      BSTreeNode *tmp = stk.top();
      stk.pop();
      node = tmp->bst.right;
    }
  }

  return res;
}

static vector<VALUE_TYPE> bstree_traverse_mid(BSTree *bs_tree) {
  if (!bs_tree || !bs_tree->root) {
    return {};
  }

  stack<BSTreeNode*> stk;
  BSTreeNode *node = bs_tree->root;

  vector<VALUE_TYPE> res;
  while (node || !stk.empty()) {
    if (node) {
      stk.push(node);
      node = node->bst.left;
    } else {
      BSTreeNode *tmp = stk.top();
      stk.pop();
      res.push_back(tmp->data);
      node = tmp->bst.right;
    }
  }

  return res;
}

static vector<VALUE_TYPE> bstree_traverse_tail(BSTree *bs_tree) {
  if (!bs_tree || !bs_tree->root) {
    return {};
  }

  stack<BSTreeNode*> stk;
  stack<BSTreeNode*> stk2;
  BSTreeNode *node = bs_tree->root;
  stk.push(node);

  while (!stk.empty()) {
    BSTreeNode *tmp = stk.top();
    stk.pop();
    stk2.push(tmp);

    if (tmp->bst.left) {
      stk.push(tmp->bst.left);
    }

    if (tmp->bst.right) {
      stk.push(tmp->bst.right);
    }
  }

  vector<VALUE_TYPE> res;
  while (!stk2.empty()) {
    BSTreeNode *tmp = stk2.top();
    stk2.pop();
    res.push_back(tmp->data);
  }

  return res;
}

static vector<VALUE_TYPE> bstree_traverse_level(BSTree *bs_tree) {
  if (!bs_tree || !bs_tree->root) {
    return {};
  }

  deque<BSTreeNode*> deq;
  BSTreeNode *node = bs_tree->root;
  deq.push_back(node);

  vector<VALUE_TYPE> res;
  while (!deq.empty()) {
    BSTreeNode *tmp = deq.front();
    deq.pop_front();

    res.push_back(tmp->data);

    if (tmp->bst.left) {
      deq.push_back(tmp->bst.left);
    }

    if (tmp->bst.right) {
      deq.push_back(tmp->bst.right);
    }
  }

  return res;
}

vector<VALUE_TYPE> bstree_traverse(BSTree *bs_tree, TraverseType traverse_type) {
  switch(traverse_type) {
    case TraverseType::kPrev:
      return bstree_traverse_prev(bs_tree);
    
    case TraverseType::kMid:
      return bstree_traverse_mid(bs_tree);

    case TraverseType::kTail:
      return bstree_traverse_tail(bs_tree);

    case TraverseType::kLevel:
      return bstree_traverse_level(bs_tree);

    default:
      return {};
  }
}
