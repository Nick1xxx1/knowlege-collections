project(testSort)

add_executable(testSort test_sort.cc)
target_link_libraries(testSort PUBLIC libgtest.a pthread)
