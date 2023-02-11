project(testBloomFilter)

add_library(BloomFilter bloom_filter.cc)
target_include_directories(BloomFilter PUBLIC ../../common/include)

add_executable(testBloomFilter test_bloom_filter.cc)
target_include_directories(testBloomFilter PUBLIC ../../common/include/gtest)
target_link_directories(testBloomFilter PUBLIC ../../common/lib/gtest)
target_link_libraries(testBloomFilter PUBLIC libgtest.a pthread BloomFilter)