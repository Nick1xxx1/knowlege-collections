#include <iostream>
#include <string>

#include "gtest/gtest.h"

#include "bloom_filter.h"

using std::cerr;
using std::cout;
using std::endl;
using std::string;

namespace {
  constexpr nc_uint32_t kMaxItemNum = 6000000;
  constexpr nc_float64_t kPositiveFalse = 0.0001;
  constexpr nc_uint32_t kTestNum = 1000;
} // namespace

int main(int argc, char **argv) {
  // printf("Running main() from %s\n\n", __FILE__);
  // testing::InitGoogleTest(&argc, argv);

  BloomFilter bloom_filter(time(NULL), kMaxItemNum, kPositiveFalse);
  char url[128] = {0};
  for (size_t i = 0; i < kTestNum; ++i) {
    snprintf(url, 128, "http://www.moloka.com/%lu.html", i);
    if (bloom_filter.AddValue((const void*)url, strlen(url)) == Result::kError) {
      cerr << "Add value to bloom filter falied" << endl;
      return -1;
    }

    memset(url, 0, sizeof(url));
  }

  // 检查某个url是否存在
  string test_url = "http://www.moloka.com/1.html";
  if (bloom_filter.CheckValue((const void*)test_url.c_str(), test_url.size()) == Result::kExist) {
    cout << "http://www.moloka.com/1.html exist!!!" << endl;
  }

  string test_url2 = "http://www.moloka.com/100000.html";
  if (bloom_filter.CheckValue((const void*)test_url2.c_str(), test_url2.size()) == Result::kNotExist) {
    cout << "http://www.moloka.com/100000.html not exist!!!" << endl;
  }

  return 0;
}
