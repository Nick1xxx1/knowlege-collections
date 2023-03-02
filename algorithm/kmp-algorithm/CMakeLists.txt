project(testKmp)

add_executable(testKmp test_kmp.cc)
add_library(kmp kmp.cc)
target_link_libraries(testKmp PUBLIC libgtest.a pthread kmp)
