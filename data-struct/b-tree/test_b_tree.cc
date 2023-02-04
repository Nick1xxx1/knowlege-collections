#include <string>

#include "gtest/gtest.h"

#include "b_tree.h"

using std::cout;
using std::endl;
using std::string;

void BTreePrint(BTree *T, BTreeNode *node, nc_int32_t layer) {
  BTreeNode *p = node;
  nc_int32_t i;
  if (!p) {
    cout << "the tree is empty" << endl;
  } else {
    cout << endl
         << "layer =" << layer << " keynum = " << p->key_num
         << " is_leaf = " << static_cast<nc_int32_t>(p->type) << endl;
    for (i = 0; i < node->key_num; i++) {
      cout << static_cast<nc_char_t>(p->keys[i]) << " ";
    }
    cout << endl;
    ++layer;
    for (i = 0; i <= p->key_num; i++) {
      if (p->children[i]) {
        BTreePrint(T, p->children[i], layer);
      }
    }
  }
}

int main(int argc, char **argv) {
  // printf("Running main() from %s\n\n", __FILE__);
  // testing::InitGoogleTest(&argc, argv);

  BTree *b_tree = BTreeCreate();

  int i = 0;
  string key = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  for (i = 0; i < 26; i++) {
    BTreeInsert(b_tree, key[i]);
  }

  BTreePrint(b_tree, b_tree->root, 0);

  BTreeDestory(b_tree);

  return 0;
}
