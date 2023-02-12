#include "b_tree.h"

#include <memory>

/**
 * @brief 创建新的B-树节点
 * 
 * @param node_type 节点的类型, 分为为普通节点(非叶子节点)和叶子节点
 * @return BTreeNode* 新创建的B-树节点
 */
static BTreeNode *BTreeCreateNode(NodeType node_type);

/**
 * @brief 销毁B-树节点
 * 
 * @param node 待销毁的B-树节点
 */
static void BTreeDestroyNode(BTreeNode *node);

/**
 * @brief 递归删除节点函数
 * 
 * @param node 待删除的节点
 */
static void BTreeTraverseDelete(BTreeNode **node);

/**
 * @brief 分裂某个节点的孩子节点
 * 
 * @param parent_node 孩子节点需要进行分裂的节点
 * @param child_index 待分裂的孩子节点的位置索引
 */
static void BTreeSplitChild(BTreeNode *parent_node, nc_int32_t child_index);

/**
 * @brief 插入一个未满的节点
 * 
 * @param node 未满的节点
 * @param key 待插入的key
 */
static void BTreeInsertNonFull(BTreeNode *node, KEY_TYPE key);

/**
 * @brief 删除节点中指定键的孩子节点
 * 
 * @param b_tree B-树对象
 * @param node 待删除节点的节点
 * @param key 键
 */
static void BTreeDeleteKey(BTree *b_tree, BTreeNode *node, KEY_TYPE key);

/**
 * @brief 节点合并, 将node的两个孩子节点以及node中的一个键值合并为一个节点
 * 
 * @param b_tree B-树对象
 * @param parent_node 用于合并以及获取孩子节点的节点
 * @param merge_index 合并节点的获取索引
 */
static void BTreeMerge(BTree *b_tree, BTreeNode *parent_node, nc_int32_t merge_index);

static BTreeNode *BTreeCreateNode(NodeType node_type) {
  BTreeNode *node = nullptr;
  node = new (std::nothrow)BTreeNode;
  if (!node) {
    return nullptr;
  }

  node->type = node_type;
  node->keys = new (std::nothrow)KEY_TYPE[kOrder - 1];
  if (!node->keys) {
    delete node;
    return nullptr;
  }

  node->children = new (std::nothrow)BTreeNode *[kOrder];
  if (!node->children) {
    delete node->keys;
    delete node;
    return nullptr;
  }

  node->key_num = 0;

  return node;
}

static void BTreeDestroyNode(BTreeNode *node) {
  if (!node) {
    return;
  }

  if (node->children) {
    delete node->children;
  }

  if (node->keys) {
    delete node->keys;
  }

  delete node;
}

static void BTreeTraverseDelete(BTreeNode **node) {
  if (!*node) {
    return;
  }

  nc_int32_t index = 0;
  for (; index < (*node)->key_num + 1; ++index) {
    if ((*node)->children[index]) {
      BTreeTraverseDelete(&(*node)->children[index]);
    }
  }

  BTreeDestroyNode(*node);
  *node = nullptr;
}

static void BTreeSplitChild(BTreeNode *parent_node, nc_int32_t child_index) {
  nc_int32_t degree = kOrder / 2;

  BTreeNode *split_node = parent_node->children[child_index];
  BTreeNode *new_node = BTreeCreateNode(split_node->type);

  new_node->key_num = degree - 1;
  split_node->key_num = degree - 1;

  nc_int32_t index = 0;
  for (index = 0; index < degree - 1; ++index) {
    new_node->keys[index] = split_node->keys[index + degree];
  }

  if (split_node->type == NodeType::kNormal) {
    for (index = 0; index < degree; ++index) {
      new_node->children[index] = split_node->children[index + degree];
    }
  }

  for (index = parent_node->key_num; index >= child_index + 1; --index) {
    parent_node->children[index + 1] = parent_node->children[index];
  }

  parent_node->children[child_index + 1] = new_node;

  for (index = parent_node->key_num - 1; index >= child_index; --index) {
    parent_node->keys[index + 1] = parent_node->keys[index];
  }
  parent_node->keys[child_index] = split_node->keys[degree - 1];
  parent_node->key_num += 1;
}

static void BTreeInsertNonFull(BTreeNode *node, KEY_TYPE key) {
  nc_int32_t index = node->key_num - 1;

  // 节点为叶子节点, 直接插入
  if (node->type == NodeType::kLeaf) {
    // 查找
    for (; index >= 0 && node->keys[index] > key; --index) {
      node->keys[index + 1] = node->keys[index];
    }

    node->keys[index + 1] = key;
    ++node->key_num;
    return;
  }

  // 节点非叶子节点, 插入到孩子节点中
  // 查找孩子节点中比key值小的最大节点
  while (index >= 0 && node->keys[index] > key) {
    --index;
  }

  if (node->children[index + 1]->key_num == kOrder - 1) {
    // 孩子节点满了, 先分裂
    BTreeSplitChild(node, index + 1);
    if (node->keys[index + 1] < key) {
      ++index;
    }
  }

  BTreeInsertNonFull(node->children[index + 1], key);
}

static void BTreeDeleteKey(BTree *b_tree, BTreeNode *node, KEY_TYPE key) {
  if (!node) {
    return;
  }

  nc_int32_t index = 0;
  nc_int32_t degree = kOrder / 2;

  while (index < node->key_num && key > node->keys[index]) {
    ++index;
  }

  if (index < node->key_num && key == node->keys[index]) {
    // 找到了键对应的节点

    if (node->type == NodeType::kLeaf) {
      // 该节点为叶子节点

      for (nc_int32_t i = index; i < node->key_num - 1; ++i) {
        node->keys[i] = node->keys[i + 1];
      }

      node->keys[node->key_num - 1] = 0;
      --node->key_num;

      if (node->key_num == 0) {
        // 节点不包含关键字了
        BTreeDestroyNode(node);
        b_tree->root = nullptr;
      }
      // 该节点不为叶子节点
    } else if (node->children[index]->key_num >= degree) {
      BTreeNode *left = node->children[index];
      node->keys[index] = left->keys[left->key_num - 1];
      BTreeDeleteKey(b_tree, left, left->keys[left->key_num - 1]);
    } else if (node->children[index + 1]->key_num >= degree) {
      BTreeNode *right = node->children[index + 1];
      node->keys[index] = right->keys[0];
      BTreeDeleteKey(b_tree, right, right->keys[0]);
    } else {
      BTreeMerge(b_tree, node, index);
      BTreeDeleteKey(b_tree, node->children[index], key);
    }

    return;
  }

  BTreeNode *child = node->children[index];
  if (!child) {
    return;
  }

  if (child->key_num != degree - 1) {
    BTreeDeleteKey(b_tree, child, key);
    return;
  }

  BTreeNode *left = nullptr;
  BTreeNode *right = nullptr;
  if (index - 1 >= 0) {
    left = node->children[index - 1];
  }
  if (index + 1 <= node->key_num) {
    right = node->children[index + 1];
  }

  if ((left && left->key_num >= degree) || (right && right->key_num >= degree)) {
    bool more_key_num_in_right = right ? true : false;
    if (left && right) {
      more_key_num_in_right = left->key_num >= right->key_num ? false : true;
    }

    if (right && right->key_num >= degree && more_key_num_in_right) {
      // 右边的节点关键字数量更多, 则从右边节点借节点
      child->keys[child->key_num] = node->keys[index];
      child->children[child->key_num + 1] = right->children[0];
      ++child->key_num;

      node->keys[index] = right->keys[0];
      for (nc_int32_t i = 0; i < right->key_num - 1; ++i) {
        right->keys[i] = right->keys[i + 1];
        right->children[i] = right->children[i + 1];
      }
      right->keys[right->key_num - 1] = 0;
      right->children[right->key_num - 1] = right->children[right->key_num];
      right->children[right->key_num] = nullptr;
      --right->key_num;
    } else {
      // 左边的节点关键字数量更多, 则从左边节点借节点
      for (nc_int32_t i = child->key_num; i > 0; --i) {
        child->keys[i] = child->keys[i - 1];
        child->children[i + 1] = child->children[i];
      }

      child->children[1] = child->children[0];
      child->children[0] = left->children[left->key_num];
      child->keys[0] = node->keys[index - 1];
      ++child->key_num;

      node->keys[index - 1] = left->keys[left->key_num - 1];
      left->keys[left->key_num - 1] = 0;
      left->children[left->key_num] = nullptr;
      --left->key_num;
    }
  } else if ((!left || left->key_num == degree - 1) && (!right || right->key_num == degree - 1)) {
    if (left && left->key_num == degree - 1) {
      BTreeMerge(b_tree, node, index - 1);
      child = left;
    } else if (right && right->key_num == degree - 1) {
      BTreeMerge(b_tree, node, index);
    }
  }

  BTreeDeleteKey(b_tree, child, key);
}

static void BTreeMerge(BTree *b_tree, BTreeNode *parent_node, nc_int32_t merge_index) {
  BTreeNode *left = parent_node->children[merge_index];
  BTreeNode *right = parent_node->children[merge_index + 1];

  nc_int32_t i = 0;
  nc_int32_t degree = kOrder / 2;

  left->keys[degree - 1] = parent_node->keys[merge_index];
  for (; i < degree - 1; ++i) {
    left->keys[degree + i] = right->keys[i];
  }
  if (left->type == NodeType::kNormal) {
    for (i = 0; i < degree; ++i) {
      left->children[degree + i] = right->children[i];
    }
  }
  left->key_num += degree;

  BTreeDestroyNode(right);

  for (i = merge_index + 1; i < parent_node->key_num; ++i) {
    parent_node->keys[i - 1] = parent_node->keys[i];
    parent_node->children[i] = parent_node->children[i + 1];
  }
  parent_node->children[i + 1] = nullptr;
  --parent_node->key_num;

  if (parent_node->key_num == 0) {
    b_tree->root = left;
    BTreeDestroyNode(parent_node);
  }
}

BTree *BTreeCreate() {
  BTree *b_tree = new(std::nothrow)BTree;
  if (!b_tree) {
    return nullptr;
  }
  b_tree->degree = kOrder / 2;

  BTreeNode *new_node = BTreeCreateNode(NodeType::kLeaf);
  if (!new_node) {
    delete b_tree;
    return nullptr;
  }

  b_tree->root = new_node;

  return b_tree;
}



void BTreeDestory(BTree *b_tree) {
  if (!b_tree) {
    return;
  }

  BTreeTraverseDelete(&b_tree->root);

  delete b_tree;
}

Result BTreeInsert(BTree *b_tree, KEY_TYPE key) {
  // 插入有两种情况, 插入只会在叶子节点进行插入
  // 1、根节点不满
  // 2、根节点满了

  if (!b_tree || !b_tree->root) {
    return Result::kError;
  }

  BTreeNode *root = b_tree->root;
  if (root->key_num != kOrder - 1) {
    BTreeInsertNonFull(root, key);
  } else {
    BTreeNode *new_node = BTreeCreateNode(NodeType::kNormal);
    b_tree->root = new_node;

    new_node->children[0] = root;
    BTreeSplitChild(new_node, 0);
    
    nc_uint32_t index = 0;
    if (new_node->keys[0] < key) {
      ++index;
    }

    BTreeInsertNonFull(new_node->children[index], key);
  }

  return Result::kOk;
}

Result BTreeDelete(BTree *b_tree, KEY_TYPE key) {
  if (!b_tree || !b_tree->root) {
    return Result::kError;
  }

  BTreeDeleteKey(b_tree, b_tree->root, key);
  return Result::kOk;
}
