#include "rb_tree.h"

/**
 * @brief 销毁红黑树的某个节点
 * 
 * @param node 待销毁的节点
 */
static void RBTreeDestoryNode(RBTreeNode *node);

/**
 * @brief 左旋函数
 * 
 * @param rb_tree 进行左旋的红黑树
 * @param x 左旋的节点
 */
static void RBTreeLeftRotate(RBTree *rb_tree, RBTreeNode *x);

/**
 * @brief 右旋函数
 * 
 * @param rb_tree 进行右旋的红黑树
 * @param y 右旋的节点
 */
static void RBTreeRightRotate(RBTree *rb_tree, RBTreeNode *y);

/**
 * @brief 插入节点后的调整函数
 * 
 * @param rb_tree 插入了节点的红黑树
 * @param adjust_node 待调整的节点, 恒为红色
 */
static void RBTreeInsertAdjust(RBTree *rb_tree, RBTreeNode *adjust_node);

/**
 * @brief 查找节点的最小子节点
 * 
 * @param rb_tree 用于查找的红黑树
 * @param node 需要查找最小子节点的节点
 * @return RBTreeNode* 最小子节点
 */
static RBTreeNode *RBTreeMini(RBTree *rb_tree, RBTreeNode *node);

/**
 * @brief 查找节点的后继节点
 * 
 * @param rb_tree 删除节点的红黑树
 * @param node 用于查找后继节点的节点
 * @return RBTreeNode* 得到的后继节点
 */
static RBTreeNode *RBTreeSuccessor(RBTree *rb_tree, RBTreeNode *node);

/**
 * @brief 红黑树删除节点后的调整函数
 * 
 * @param rb_tree 待调整的红黑树
 * @param node 删除后的调整节点
 */
static void RBTreeDeleteAdjust(RBTree *rb_tree, RBTreeNode *node);

static void RBTreeDestoryNode(RBTreeNode *node) {
  if (!node) {
    return;
  }

  // 若左子树存在, 则一直递归删除左子树
  if (node->rbt.left) {
    RBTreeDestoryNode(node->rbt.left);
    node->rbt.left = nullptr;
  }

  // 若右子树存在, 则一直递归删除右子树
  if (node->rbt.right) {
    RBTreeDestoryNode(node->rbt.right);
    node->rbt.right = nullptr;
  }

  if (node) {
    delete node;
    node = nullptr;
  }
}

static void RBTreeLeftRotate(RBTree *rb_tree, RBTreeNode *x) {
  RBTreeNode *y = x->rbt.right;

  // step1: x的右子树指向y的左子树
  x->rbt.right = y->rbt.left;
  // step2: 如果y的左子树不为nil, 那么将它的父节点更新为x
  if (y->rbt.left != rb_tree->nil) {
    y->rbt.left->rbt.parent = x;
  }

  // step3: 将y的父节点变为x的父节点
  y->rbt.parent = x->rbt.parent;
  // step4: 更新x父节点的子节点
  if (x->rbt.parent == rb_tree->nil) {
    // 如果x的父节点为nil, 即x为根节点, 则直接将y更新为新的根节点
    rb_tree->root = y;
  } else if (x == x->rbt.parent->rbt.left) {
    // 如果x为其父节点的左子树, 则将y更新为x父节点的左子树
    x->rbt.parent->rbt.left = y;
  } else {
    // 如果x为其父节点的右子树, 则将y更新为x父节点的右子树
    x->rbt.parent->rbt.right = y;
  }

  // step5: 更新y的左子树
  y->rbt.left = x;
  // step6: 更新x的父节点
  x->rbt.parent = y;
}

static void RBTreeRightRotate(RBTree *rb_tree, RBTreeNode *y) {
  RBTreeNode *x = y->rbt.left;

  // step1: y的左子树指向x的右子树
  y->rbt.left = x->rbt.right;
  // step2: 如果x的右子树不为nil, 那么将它的父节点更新为y
  if (x->rbt.right != rb_tree->nil) {
    x->rbt.right->rbt.parent = y;
  }

  // step3: 将x的父节点变为y的父节点
  x->rbt.parent = y->rbt.parent;
  // step4: 更新y父节点的子节点
  if (y->rbt.parent == rb_tree->nil) {
    // 如果y的父节点为nil, 即y为根节点, 则直接将x更新为新的根节点
    rb_tree->root = x;
  } else if (y == y->rbt.parent->rbt.right) {
    // 如果y为其父节点的左子树, 则将x更新为y父节点的右子树
    y->rbt.parent->rbt.right = x;
  } else {
    // 如果y为其父节点的左子树, 则将x更新为y父节点的左子树
    y->rbt.parent->rbt.left = x;
  }

  // step5: 更新x的右子树
  x->rbt.right = y;
  // step6: 更新y的父节点
  y->rbt.parent = x;
}

static void RBTreeInsertAdjust(RBTree *rb_tree, RBTreeNode *adjust_node) {
  // 只有当父节点为红色时才进行调整, 有几种情况:
  // 1、叔父节点为红色, 直接变色
  // 2、叔父节点为祖父节点的右节点, 叔父节点为黑色, 调整节点为父节点的右子树, 先左旋再右旋
  // 3、叔父节点为祖父节点的左节点, 叔父节点为黑色, 调整节点为父节点的左子树, 先右旋再左旋
  
  // 父节点为红色时才需调整
  while (adjust_node->rbt.parent->node_color == NodeColor::kRed) {
    if (adjust_node->rbt.parent == adjust_node->rbt.parent->rbt.parent->rbt.left) {
      RBTreeNode *uncle_node = adjust_node->rbt.parent->rbt.parent->rbt.right;

      if (uncle_node->node_color == NodeColor::kRed) {
        // 情况1, 直接变色即可
        adjust_node->rbt.parent->node_color = NodeColor::kBlack;
        uncle_node->node_color = NodeColor::kBlack;
        adjust_node->rbt.parent->rbt.parent->node_color = NodeColor::kRed;

        adjust_node = adjust_node->rbt.parent->rbt.parent;
      } else {
        // 情况2
        if (adjust_node == adjust_node->rbt.parent->rbt.right) {
          adjust_node = adjust_node->rbt.parent;
          RBTreeLeftRotate(rb_tree, adjust_node);
        }
        
        adjust_node->rbt.parent->node_color = NodeColor::kBlack;
        adjust_node->rbt.parent->rbt.parent->node_color = NodeColor::kRed;
        RBTreeRightRotate(rb_tree, adjust_node->rbt.parent->rbt.parent);
      }
    } else {
      RBTreeNode *uncle_node = adjust_node->rbt.parent->rbt.parent->rbt.left;

      if (uncle_node->node_color == NodeColor::kRed) {
        // 情况1
        adjust_node->rbt.parent->node_color = NodeColor::kBlack;
        uncle_node->node_color = NodeColor::kBlack;
        adjust_node->rbt.parent->rbt.parent->node_color = NodeColor::kRed;

        adjust_node = adjust_node->rbt.parent->rbt.parent;
      } else {
        // 情况3
        if (adjust_node == adjust_node->rbt.parent->rbt.left) {
          adjust_node = adjust_node->rbt.parent;
          RBTreeRightRotate(rb_tree, adjust_node);
        }
        
        adjust_node->rbt.parent->node_color = NodeColor::kBlack;
        adjust_node->rbt.parent->rbt.parent->node_color = NodeColor::kRed;
        RBTreeLeftRotate(rb_tree, adjust_node->rbt.parent->rbt.parent);
      }
    }
  }

  rb_tree->root->node_color = NodeColor::kBlack;
}

static RBTreeNode *RBTreeMini(RBTree *rb_tree, RBTreeNode *node) {
  if (!rb_tree || !node) {
    return nullptr;
  }

  while (node->rbt.left != rb_tree->nil) {
    node = node->rbt.left;
  }

  return node;
}

static RBTreeNode *RBTreeSuccessor(RBTree *rb_tree, RBTreeNode *node) {
  if (node->rbt.right != rb_tree->nil) {
    return RBTreeMini(rb_tree, node->rbt.right);
  }

  RBTreeNode *successor_node = node->rbt.parent;
  while ((successor_node != rb_tree->nil) && (node == successor_node->rbt.right)) {
    node = successor_node;
    successor_node = successor_node->rbt.parent;
  }

  return successor_node;
}

static void RBTreeDeleteAdjust(RBTree *rb_tree, RBTreeNode *node) {
  while ((node != rb_tree->nil) && (node->node_color == NodeColor::kBlack)) {
    if (node == node->rbt.parent->rbt.left) {
      RBTreeNode *tmp_node = node->rbt.parent->rbt.right;
      if (tmp_node->node_color == NodeColor::kRed) {
        tmp_node->node_color = NodeColor::kBlack;
        node->rbt.parent->node_color = NodeColor::kRed;

        RBTreeLeftRotate(rb_tree, node->rbt.parent);
        tmp_node = node->rbt.parent->rbt.right;
      }

      if ((tmp_node->rbt.left->node_color == NodeColor::kBlack) && (tmp_node->rbt.right->node_color == NodeColor::kBlack)) {
        tmp_node->node_color = NodeColor::kRed;
        node = node->rbt.parent;
      } else {
        if (tmp_node->rbt.right->node_color == NodeColor::kBlack) {
          tmp_node->rbt.left->node_color = NodeColor::kBlack;
          tmp_node->node_color = NodeColor::kRed;
          RBTreeRightRotate(rb_tree, tmp_node);
          tmp_node = node->rbt.parent->rbt.right;
        }

        tmp_node->node_color = node->rbt.parent->node_color;
        node->rbt.parent->node_color = NodeColor::kBlack;
        tmp_node->rbt.right->node_color = NodeColor::kBlack;
        RBTreeLeftRotate(rb_tree, node->rbt.parent);

        node = rb_tree->root;
      }
    } else {
      RBTreeNode *tmp_node = node->rbt.parent->rbt.left;
      if (tmp_node->node_color == NodeColor::kRed) {
        tmp_node->node_color = NodeColor::kBlack;
        node->rbt.parent->node_color = NodeColor::kRed;

        RBTreeRightRotate(rb_tree, node->rbt.parent);
        tmp_node = node->rbt.parent->rbt.left;
      }

      if ((tmp_node->rbt.left->node_color == NodeColor::kBlack) && (tmp_node->rbt.right->node_color == NodeColor::kBlack)) {
        tmp_node->node_color = NodeColor::kRed;
        node = node->rbt.parent;
      } else {
        if (tmp_node->rbt.left->node_color == NodeColor::kBlack) {
          tmp_node->rbt.right->node_color = NodeColor::kBlack;
          tmp_node->node_color = NodeColor::kRed;
          RBTreeLeftRotate(rb_tree, tmp_node);
          tmp_node = node->rbt.parent->rbt.left;
        }

        tmp_node->node_color = node->rbt.parent->node_color;
        node->rbt.parent->node_color = NodeColor::kBlack;
        tmp_node->rbt.left->node_color = NodeColor::kBlack;
        RBTreeRightRotate(rb_tree, node->rbt.parent);

        node = rb_tree->root;
      }
    }
  }

  node->node_color = NodeColor::kBlack;
}

RBTree *RBTreeCreate() {
  RBTree *rb_tree = new RBTree;
  rb_tree->nil = new RBTreeNode;
  rb_tree->nil->node_color = NodeColor::kBlack;
  rb_tree->root = rb_tree->nil;

  return rb_tree;
}

void RBTreeDestory(RBTree *rb_tree) {
  if (!rb_tree) {
    return;
  }

  if (!rb_tree->root) {
    if (rb_tree->nil) {
      delete rb_tree->nil;
    }

    delete rb_tree;
    return;
  }

  RBTreeNode *root = rb_tree->root;
  RBTreeDestoryNode(root);

  delete rb_tree;
}

void RBTreeInsert(RBTree *rb_tree, RBTreeNode *insert_node) {
  RBTreeNode *tmp_node = rb_tree->nil;
  RBTreeNode *node = rb_tree->root;

  while (node != rb_tree->nil) {
    tmp_node = node;
    if (node->key == insert_node->key) {
      return;
    }

    if (node->key < insert_node->key) {
      node = node->rbt.right;
    } else {
      node = node->rbt.left;
    }
  }

  insert_node->rbt.parent = tmp_node;
  if (tmp_node == rb_tree->nil) {
    rb_tree->root = insert_node;
  } else if (tmp_node->key > insert_node->key) {
    tmp_node->rbt.left = insert_node;
  } else {
    tmp_node->rbt.right = insert_node;
  }

  insert_node->rbt.left = rb_tree->nil;
  insert_node->rbt.right = rb_tree->nil;
  insert_node->node_color = NodeColor::kRed;

  // 插入后进行调整
  RBTreeInsertAdjust(rb_tree, insert_node);
}

RBTreeNode *RBTreeDelete(RBTree *rb_tree, RBTreeNode *node) {
  if (!rb_tree || !node) {
    return nullptr;
  }

  RBTreeNode *del_node = rb_tree->nil;
  RBTreeNode *adjust_node = rb_tree->nil;

  if (del_node->rbt.left == rb_tree->nil || del_node->rbt.right == rb_tree->nil) {
    del_node = node;
  } else {
    del_node = RBTreeSuccessor(rb_tree, node);
  }

  if (del_node->rbt.left != rb_tree->nil) {
    adjust_node = del_node->rbt.left;
  } else if (del_node->rbt.right != rb_tree->nil) {
    adjust_node = del_node->rbt.right;
  }

  adjust_node->rbt.parent = del_node->rbt.parent;
  if (del_node->rbt.parent == rb_tree->nil) {
    rb_tree->root = adjust_node;
  } else if (del_node == del_node->rbt.parent->rbt.left) {
    del_node->rbt.parent->rbt.left = adjust_node;
  } else {
    del_node->rbt.parent->rbt.right = adjust_node;
  }

  if (del_node != node) {
    node->key = del_node->key;
    node->value = del_node->value;
  }

  if (del_node->node_color == NodeColor::kBlack) {
    RBTreeDeleteAdjust(rb_tree, adjust_node);
  }

  return del_node;
}

RBTreeNode *RBTreeSearch(RBTree *rb_tree, KEY_TYPE key) {
  RBTreeNode *node = rb_tree->root;

  while (node != rb_tree->nil) {
    if (node->key == key) {
      return node;
    }

    if (key < node->key) {
      node = node->rbt.left;
    } else {
      node = node->rbt.right;
    }
  }

  return rb_tree->nil;
}
