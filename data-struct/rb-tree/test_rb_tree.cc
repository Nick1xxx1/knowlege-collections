#include <iostream>
#include <vector>

#include "gtest/gtest.h"

#include "rb_tree.h"

using std::cout;
using std::endl;
using std::vector;

static void RBTreeTraverse(RBTree *rb_tree, RBTreeNode *node) {
  if (node == rb_tree->nil) {
    return;
  }

  RBTreeTraverse(rb_tree, node->rbt.left);
  cout << "key: " << node->key << ", color: " << static_cast<nc_int32_t>(node->node_color) << endl;
  RBTreeTraverse(rb_tree, node->rbt.right);
}

int main(int argc, char **argv) {
  vector<nc_int32_t> key_arr = {24, 25, 13, 35, 23, 26, 67, 47, 38, 98,
                                20, 19, 17, 49, 12, 21, 9,  18, 14, 15};
  RBTree *rb_tree = RBTreeCreate();

  for (nc_uint32_t i = 0; i != key_arr.size(); ++i) {
    RBTreeNode *node = new RBTreeNode;
    node->key = key_arr[i];
    node->value = nullptr;

    RBTreeInsert(rb_tree, node);
  }

  RBTreeTraverse(rb_tree, rb_tree->root);

  cout << "----------------------------------------" << endl;

	for (nc_uint32_t i = 0; i != key_arr.size(); ++i) {

		RBTreeNode *node = RBTreeSearch(rb_tree, key_arr[i]);
    RBTreeDelete(rb_tree, node);

		RBTreeTraverse(rb_tree, rb_tree->root);
		cout << "----------------------------------------" << endl;
	}

  RBTreeDestory(rb_tree);

  return 0;
}
