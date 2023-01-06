project(testRedBlackTree)

add_library(rebBlackTree rb_tree.cc)
target_include_directories(rebBlackTree PUBLIC ../../common/include)

add_executable(testRBTree test_rb_tree.cc)
target_include_directories(testRBTree PUBLIC ../../common/include/gtest)
target_link_directories(testRBTree PUBLIC ../../common/lib/gtest)
target_link_libraries(testRBTree PUBLIC libgtest.a pthread rebBlackTree)
