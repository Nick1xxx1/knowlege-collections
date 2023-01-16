project(testBTree)

add_library(BTree b_tree.cc)
target_include_directories(BTree PUBLIC ../../common/include)

add_executable(testBTree test_b_tree.cc)
target_include_directories(testBTree PUBLIC ../../common/include/gtest)
target_link_directories(testBTree PUBLIC ../../common/lib/gtest)
target_link_libraries(testBTree PUBLIC libgtest.a pthread BTree)
