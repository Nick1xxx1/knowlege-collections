#include "gtest/gtest.h"

#include "b_tree.h"

int main(int argc, char **argv) {
  printf("Running main() from %s\n\n", __FILE__);
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
