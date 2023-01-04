project(testRedBlackTree)

add_library(rebBlackTree rb_tree.cc)
target_include_directories(rebBlackTree PUBLIC ../../common/include)
