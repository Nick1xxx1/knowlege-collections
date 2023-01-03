#include <vector>

#include "gtest/gtest.h"

#include "binary_search_tree.h"

using std::vector;

TEST(testBinarySearchTree, testCreateAndDestory) {
  vector<VALUE_TYPE> dataset = {24, 25, 13};
  BSTree *bs_tree = new BSTree;
  bs_tree->root = nullptr;
  for (auto data : dataset) {
    EXPECT_EQ(bstree_insert(bs_tree, data), kOk);
  }

  bstree_destory(&bs_tree);
  EXPECT_EQ(bs_tree, nullptr);
}

TEST(testBinarySearchTree, testTraversePrev) {
  vector<VALUE_TYPE> dataset = {24, 25, 13};
  BSTree *bs_tree = new BSTree;
  bs_tree->root = nullptr;
  for (auto data : dataset) {
    bstree_insert(bs_tree, data);
  }

  vector<VALUE_TYPE> expect = {24, 13, 25};
  vector<VALUE_TYPE> res = bstree_traverse(bs_tree, TraverseType::kPrev);

  EXPECT_EQ(expect, res);

  bstree_destory(&bs_tree);
}

TEST(testBinarySearchTree, testTraverseMid) {
  vector<VALUE_TYPE> dataset = {24, 25, 13};
  BSTree *bs_tree = new BSTree;
  bs_tree->root = nullptr;
  for (auto data : dataset) {
    bstree_insert(bs_tree, data);
  }

  vector<VALUE_TYPE> expect = {13, 24, 25};
  vector<VALUE_TYPE> res = bstree_traverse(bs_tree, TraverseType::kMid);

  EXPECT_EQ(expect, res);

  bstree_destory(&bs_tree);
}

TEST(testBinarySearchTree, testTraverseTail) {
  vector<VALUE_TYPE> dataset = {24, 25, 13};
  BSTree *bs_tree = new BSTree;
  bs_tree->root = nullptr;
  for (auto data : dataset) {
    bstree_insert(bs_tree, data);
  }

  vector<VALUE_TYPE> expect = {13, 25, 24};
  vector<VALUE_TYPE> res = bstree_traverse(bs_tree, TraverseType::kTail);

  EXPECT_EQ(expect, res);

  bstree_destory(&bs_tree);
}

TEST(testBinarySearchTree, testTraverseLevel) {
  vector<VALUE_TYPE> dataset = {24, 25, 13};
  BSTree *bs_tree = new BSTree;
  bs_tree->root = nullptr;
  for (auto data : dataset) {
    bstree_insert(bs_tree, data);
  }

  vector<VALUE_TYPE> expect = {24, 13, 25};
  vector<VALUE_TYPE> res = bstree_traverse(bs_tree, TraverseType::kLevel);

  EXPECT_EQ(expect, res);

  bstree_destory(&bs_tree);
}

int main(int argc, char **argv) {
  printf("Running main() from %s\n\n", __FILE__);
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
