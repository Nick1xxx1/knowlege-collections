project(testBinarySearchTree)

add_library(binarySearchTree binary_search_tree.cc)
target_include_directories(binarySearchTree PUBLIC ../../common/include)

add_executable(testBinarySearchTree test_binary_search_tree.cc)
target_include_directories(testBinarySearchTree PUBLIC ../../common/include/gtest)
target_link_directories(testBinarySearchTree PUBLIC ../../common/lib/gtest)
target_link_libraries(testBinarySearchTree PUBLIC libgtest.a pthread binarySearchTree)
