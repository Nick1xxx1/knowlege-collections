#include <vector>

#include "gtest/gtest.h"

#include "binary_search_tree.h"

using std::vector;

TEST(testBinarySearchTree, testCreateAndDestory) {
  vector<KEY_TYPE> dataset = {24, 25, 13};
  BSTree *bs_tree = new BSTree;
  for (auto data : dataset) {
    EXPECT_EQ(BSTreeInsert(bs_tree, data), kOk);
  }

  BSTreeDestroy(&bs_tree);
  EXPECT_EQ(bs_tree, nullptr);
}

TEST(testBinarySearchTree, testTraversePrev) {
  vector<KEY_TYPE> dataset = {24, 25, 13};
  BSTree *bs_tree = new BSTree;
  for (auto data : dataset) {
    BSTreeInsert(bs_tree, data);
  }

  vector<KEY_TYPE> expect = {24, 13, 25};
  vector<KEY_TYPE> res = BSTreeTraverse(bs_tree, TraverseType::kPrev);

  EXPECT_EQ(expect, res);

  BSTreeDestroy(&bs_tree);
}

TEST(testBinarySearchTree, testTraverseMid) {
  vector<KEY_TYPE> dataset = {24, 25, 13};
  BSTree *bs_tree = new BSTree;
  for (auto data : dataset) {
    BSTreeInsert(bs_tree, data);
  }

  vector<KEY_TYPE> expect = {13, 24, 25};
  vector<KEY_TYPE> res = BSTreeTraverse(bs_tree, TraverseType::kMid);

  EXPECT_EQ(expect, res);

  BSTreeDestroy(&bs_tree);
}

TEST(testBinarySearchTree, testTraverseTail) {
  vector<KEY_TYPE> dataset = {24, 25, 13};
  BSTree *bs_tree = new BSTree;
  for (auto data : dataset) {
    BSTreeInsert(bs_tree, data);
  }

  vector<KEY_TYPE> expect = {13, 25, 24};
  vector<KEY_TYPE> res = BSTreeTraverse(bs_tree, TraverseType::kTail);

  EXPECT_EQ(expect, res);

  BSTreeDestroy(&bs_tree);
}

TEST(testBinarySearchTree, testTraverseLevel) {
  vector<KEY_TYPE> dataset = {24, 25, 13};
  BSTree *bs_tree = new BSTree;
  for (auto data : dataset) {
    BSTreeInsert(bs_tree, data);
  }

  vector<KEY_TYPE> expect = {24, 13, 25};
  vector<KEY_TYPE> res = BSTreeTraverse(bs_tree, TraverseType::kLevel);

  EXPECT_EQ(expect, res);

  BSTreeDestroy(&bs_tree);
}

TEST(testBinarySearchTree, testSearch) {
  vector<KEY_TYPE> dataset = {24, 25, 13};
  BSTree *bs_tree = new BSTree;
  for (auto data : dataset) {
    BSTreeInsert(bs_tree, data);
  }

  BSTreeNode *node = BSTreeSearch(bs_tree, 13);
  EXPECT_EQ(node->key, 13);

  node = BSTreeSearch(bs_tree, 3);
  EXPECT_EQ(node, nullptr);

  node = BSTreeSearch(nullptr, 3);
  EXPECT_EQ(node, nullptr);

  BSTreeDestroy(&bs_tree);
}

TEST(testBinarySearchTree, testDeleteMiddle) {
  vector<KEY_TYPE> dataset = {5, 3, 6, 2, 4, 7};
  BSTree *bs_tree = new BSTree;
  for (auto data : dataset) {
    BSTreeInsert(bs_tree, data);
  }

  bs_tree->root = BSTreeDelete(bs_tree, 3);
  vector<KEY_TYPE> expect = {5, 4, 6, 2, 7};
  vector<KEY_TYPE> res = BSTreeTraverse(bs_tree, TraverseType::kLevel);

  BSTreeDestroy(&bs_tree);
}

TEST(testBinarySearchTree, testDeleteNotExist) {
  vector<KEY_TYPE> dataset = {5, 3, 6, 2, 4, 7};
  BSTree *bs_tree = new BSTree;
  for (auto data : dataset) {
    BSTreeInsert(bs_tree, data);
  }

  bs_tree->root = BSTreeDelete(bs_tree, 11);
  vector<KEY_TYPE> expect = {5, 3, 6, 2, 4, 7};
  vector<KEY_TYPE> res = BSTreeTraverse(bs_tree, TraverseType::kLevel);

  BSTreeDestroy(&bs_tree);
}

TEST(testBinarySearchTree, testDeleteRoot) {
  vector<KEY_TYPE> dataset = {5, 3, 6, 2, 4, 7};
  BSTree *bs_tree = new BSTree;
  for (auto data : dataset) {
    BSTreeInsert(bs_tree, data);
  }

  bs_tree->root = BSTreeDelete(bs_tree, 5);
  vector<KEY_TYPE> expect = {6, 3, 7, 2, 4};
  vector<KEY_TYPE> res = BSTreeTraverse(bs_tree, TraverseType::kLevel);

  BSTreeDestroy(&bs_tree);
}

TEST(testBinarySearchTree, testDeleteLeaf) {
  vector<KEY_TYPE> dataset = {5, 3, 6, 2, 4, 7};
  BSTree *bs_tree = new BSTree;
  for (auto data : dataset) {
    BSTreeInsert(bs_tree, data);
  }

  bs_tree->root = BSTreeDelete(bs_tree, 7);
  vector<KEY_TYPE> expect = {5, 3, 6, 2, 4};
  vector<KEY_TYPE> res = BSTreeTraverse(bs_tree, TraverseType::kLevel);

  BSTreeDestroy(&bs_tree);
}

TEST(testBinarySearchTree, testModify) {
  vector<KEY_TYPE> dataset = {5, 3, 6, 2, 4, 7};
  BSTree *bs_tree = new BSTree;
  for (auto data : dataset) {
    BSTreeInsert(bs_tree, data);
  }

  nc_int32_t value = 4;
  nc_int8_t ret = BSTreeModify(bs_tree, 5, (void*)&value);
  EXPECT_EQ(ret, kOk);
  BSTreeNode *node = BSTreeSearch(bs_tree, 5);
  EXPECT_EQ(*(nc_int32_t*)node->value, 4);

  ret = BSTreeModify(bs_tree, 11, reinterpret_cast<void*>(value));
  EXPECT_EQ(ret, kError);

  BSTreeDestroy(&bs_tree);
}

int main(int argc, char **argv) {
  printf("Running main() from %s\n\n", __FILE__);
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
