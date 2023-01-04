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
 * @param key 赋予给节点的键
 * @return BSTreeNode* 创建的节点 
 */
static BSTreeNode *BSTreeCreateNode(KEY_TYPE key);

/**
 * @brief 删除二叉搜索树中的某个节点
 * 
 * @param node 待删除的节点
 * @param key 待删除节点对应的键
 * @return BSTreeNode* 删除后得到的替换节点
 */
static BSTreeNode *BSTreeDeleteNode(BSTreeNode *node, KEY_TYPE key);

/**
 * @brief 销毁节点函数
 * 
 * @param bs_tree_node 待销毁的节点
 */
static void BSTreeDestroyNode(BSTreeNode *bs_tree_node);

/**
 * @brief 二叉搜索树前序遍历
 * 
 * @param bs_tree 待遍历的二叉搜索树
 * @return std::vector<KEY_TYPE> 存放遍历得到的键
 */
static vector<KEY_TYPE> BSTreeTraversePrev(BSTree *bs_tree);

/**
 * @brief 二叉搜索树中序遍历
 * 
 * @param bs_tree 待遍历的二叉搜索树
 * @return std::vector<KEY_TYPE> 存放遍历得到的键
 */
static vector<KEY_TYPE> BSTreeTraverseMid(BSTree *bs_tree);

/**
 * @brief 二叉搜索树后序遍历
 * 
 * @param bs_tree 待遍历的二叉搜索树
 * @return std::vector<KEY_TYPE> 存放遍历得到的键
 */
static vector<KEY_TYPE> BSTreeTraverseTail(BSTree *bs_tree);

/**
 * @brief 二叉搜索树层序遍历
 * 
 * @param bs_tree 待遍历的二叉搜索树
 * @return std::vector<KEY_TYPE> 存放遍历得到的键
 */
static vector<KEY_TYPE> BSTreeTraverseLevel(BSTree *bs_tree);

static BSTreeNode *BSTreeCreateNode(KEY_TYPE key) {
  BSTreeNode *node = nullptr;
  try {
    node = new BSTreeNode;
  } catch (std::bad_alloc &ex) {
    return nullptr;
  }

  node->key = key;
  node->bst.left = node->bst.right = nullptr;

  return node;
}

nc_int8_t BSTreeInsert(BSTree *bs_tree, KEY_TYPE key) {
  if (!bs_tree) {
    return kError;
  }

  if (!bs_tree->root) {
    bs_tree->root = BSTreeCreateNode(key);
    return kOk;
  }

  BSTreeNode *node = bs_tree->root;
  BSTreeNode *tmp = nullptr;

  while (node) {
    tmp = node;

    if (key < node->key) {
      node = node->bst.left;
    } else if (key > node->key) {
      node = node->bst.right;
    } else {
      cerr << "Duplicate node" << endl;
      return kError;
    }
  }

  if (key < tmp->key) {
    tmp->bst.left = BSTreeCreateNode(key);
  } else {
    tmp->bst.right = BSTreeCreateNode(key);
  }

  return kOk;
}

static BSTreeNode *BSTreeDeleteNode(BSTreeNode *root, KEY_TYPE key) {
  if (!root) {
    return nullptr;
  }

  if (key > root->key) {
    root->bst.right = BSTreeDeleteNode(root->bst.right, key);
  } else if (key < root->key) {
    root->bst.left = BSTreeDeleteNode(root->bst.left, key);
  } else {
    if (!root->bst.left) {
      BSTreeNode *right_node = root->bst.right;
      delete root;
      return right_node;
    }

    if (!root->bst.right) {
      BSTreeNode *left_node = root->bst.right;
      delete root;
      return left_node;
    }
    
    BSTreeNode *node = root->bst.right;
    while (node->bst.left) {
      node = node->bst.left;
    }
    node->bst.left = root->bst.left;
    BSTreeNode *right_node = root->bst.right;
    delete root;
    root = right_node;
  }

  return root;
}

BSTreeNode *BSTreeDelete(BSTree *bs_tree, KEY_TYPE key) {
  if (!bs_tree || !bs_tree->root) {
    return nullptr;
  }

  BSTreeNode *node = BSTreeSearch(bs_tree, key);
  if (!node) {
    return bs_tree->root;
  }

  BSTreeNode *root = bs_tree->root;
  return BSTreeDeleteNode(root, key);
}

BSTreeNode *BSTreeSearch(BSTree *bs_tree, KEY_TYPE key) {
  if (!bs_tree || !bs_tree->root) {
    return nullptr;
  }

  BSTreeNode *node = bs_tree->root;
  while (node) {
    if (node->key == key) {
      return node;
    }

    if (node->key > key) {
      node = node->bst.left;
    } else {
      node = node->bst.right;
    }
  }

  return nullptr;
}

nc_int8_t BSTreeModify(BSTree *bs_tree, KEY_TYPE search_key, void *mod_value) {
  BSTreeNode *node = BSTreeSearch(bs_tree, search_key);
  if (!node) {
    return kError;
  }

  node->value = mod_value;

  return kOk;
}

static void BSTreeDestroyNode(BSTreeNode *bs_tree_node) {
  if (!bs_tree_node) {
    return;
  }

  if (bs_tree_node->bst.left) {
    BSTreeDestroyNode(bs_tree_node->bst.left);
    bs_tree_node->bst.left = nullptr;
  }

  if (bs_tree_node->bst.right) {
    BSTreeDestroyNode(bs_tree_node->bst.right);
    bs_tree_node->bst.right = nullptr;
  }

  if (bs_tree_node) {
    delete bs_tree_node;
    bs_tree_node = nullptr;
  }
}

void BSTreeDestroy(BSTree **bs_tree) {
  if (!*bs_tree) {
    return;
  }

  if (!(*bs_tree)->root) {
    delete bs_tree;
    *bs_tree = nullptr;
    return;
  }

  BSTreeDestroyNode((*bs_tree)->root);

  delete *bs_tree;
  *bs_tree = nullptr;
}

static vector<KEY_TYPE> BSTreeTraversePrev(BSTree *bs_tree) {
  if (!bs_tree || !bs_tree->root) {
    return {};
  }

  stack<BSTreeNode*> stk;
  BSTreeNode *node = bs_tree->root;

  vector<KEY_TYPE> res;
  while (node || !stk.empty()) {
    if (node) {
      res.push_back(node->key);
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

static vector<KEY_TYPE> BSTreeTraverseMid(BSTree *bs_tree) {
  if (!bs_tree || !bs_tree->root) {
    return {};
  }

  stack<BSTreeNode*> stk;
  BSTreeNode *node = bs_tree->root;

  vector<KEY_TYPE> res;
  while (node || !stk.empty()) {
    if (node) {
      stk.push(node);
      node = node->bst.left;
    } else {
      BSTreeNode *tmp = stk.top();
      stk.pop();
      res.push_back(tmp->key);
      node = tmp->bst.right;
    }
  }

  return res;
}

static vector<KEY_TYPE> BSTreeTraverseTail(BSTree *bs_tree) {
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

  vector<KEY_TYPE> res;
  while (!stk2.empty()) {
    BSTreeNode *tmp = stk2.top();
    stk2.pop();
    res.push_back(tmp->key);
  }

  return res;
}

static vector<KEY_TYPE> BSTreeTraverseLevel(BSTree *bs_tree) {
  if (!bs_tree || !bs_tree->root) {
    return {};
  }

  deque<BSTreeNode*> deq;
  BSTreeNode *node = bs_tree->root;
  deq.push_back(node);

  vector<KEY_TYPE> res;
  while (!deq.empty()) {
    BSTreeNode *tmp = deq.front();
    deq.pop_front();

    res.push_back(tmp->key);

    if (tmp->bst.left) {
      deq.push_back(tmp->bst.left);
    }

    if (tmp->bst.right) {
      deq.push_back(tmp->bst.right);
    }
  }

  return res;
}

vector<KEY_TYPE> BSTreeTraverse(BSTree *bs_tree, TraverseType traverse_type) {
  switch(traverse_type) {
    case TraverseType::kPrev:
      return BSTreeTraversePrev(bs_tree);
    
    case TraverseType::kMid:
      return BSTreeTraverseMid(bs_tree);

    case TraverseType::kTail:
      return BSTreeTraverseTail(bs_tree);

    case TraverseType::kLevel:
      return BSTreeTraverseLevel(bs_tree);

    default:
      break;
  }

  return {};
}
