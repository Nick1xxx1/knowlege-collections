#include "b_tree.h"

#include <memory>

/**
 * @brief 创建新的B-树节点
 * 
 * @param degree B-树的度, 为阶数的一半
 * @param node_type 节点的类型, 分为为普通节点(非叶子节点)和叶子节点
 * @return BTreeNode* 新创建的B-树节点
 */
static BTreeNode *BTreeCreateNode(nc_uint32_t degree, NodeType node_type);

/**
 * @brief 销毁B-树节点
 * 
 * @param node 待销毁的B-树节点
 */
static void BTreeDestroyNode(BTreeNode *node);

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

static BTreeNode *BTreeCreateNode(nc_uint32_t degree, NodeType node_type) {
  BTreeNode *node = nullptr;
  node = new (std::nothrow)BTreeNode;
  if (!node) {
    return nullptr;
  }

  node->type = node_type;
  node->keys = new (std::nothrow)KEY_TYPE[2 * degree - 1];
  if (!node->keys) {
    delete node;
    return nullptr;
  }

  node->children = new (std::nothrow)BTreeNode *[2 * degree];
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

static void BTreeSplitChild(BTreeNode *parent_node, nc_int32_t child_index) {
  BTreeNode *split_node = parent_node->children[child_index];
  nc_int32_t degree = kDegree;
  BTreeNode *new_node = BTreeCreateNode(degree, split_node->type);

  new_node->key_num = degree - 1;

  nc_int32_t index = 0;
  for (; index < degree - 1; ++index) {
    new_node->keys[index] = split_node->keys[index + degree];
  }

  if (split_node->type == NodeType::kNormal) {
    for (index = 0; index < degree; ++index) {
      new_node->children[index] = split_node->children[index + degree];
    }
  }
  split_node->key_num = degree - 1;

  for (index = parent_node->key_num; index >= child_index + 1; --index) {
    parent_node[index + 1] = parent_node[index];
  }
  parent_node->children[index + 1] = new_node;

  for (index = parent_node->key_num - 1; index >= child_index; --index) {
    parent_node->keys[index + 1] = parent_node->keys[index];
  }
  parent_node->keys[index] = split_node->keys[index - 1];
  ++parent_node->key_num;
}

static void BTreeInsertNonFull(BTreeNode *node, KEY_TYPE key) {
  nc_int32_t index = node->key_num - 1;
  nc_int32_t degree = kDegree;

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

  if (node->children[index + 1]->key_num == static_cast<nc_uint32_t>(2 * degree - 1)) {
    // 孩子节点满了, 先分裂
    BTreeSplitChild(node, index + 1);
    if (node->keys[index + 1] < key) {
      ++index;
    }
  }

  BTreeInsertNonFull(node->children[index + 1], key);
}

nc_int8_t BTreeInsert(BTree *b_tree, KEY_TYPE key) {
  // 插入有两种情况, 插入只会在叶子节点进行插入
  // 1、根节点不满
  // 2、根节点满了

  if (!b_tree || !b_tree->root) {
    return kError;
  }

  BTreeNode *root = b_tree->root;
  if (root->key_num != 2 * b_tree->degree - 1) {
    BTreeInsertNonFull(root, key);
  } else {
    BTreeNode *new_node = BTreeCreateNode(kDegree, NodeType::kNormal);
    b_tree->root = new_node;

    new_node->children[0] = root;
    BTreeSplitChild(new_node, 0);
    
    nc_uint32_t index = 0;
    if (new_node->keys[0] < key) {
      ++index;
    }

    BTreeInsertNonFull(new_node->children[index], key);
  }

  return kOk;
}
