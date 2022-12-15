#include "kmp.h"

using std::string;
using std::vector;

/**
 * @brief 生成next数组
 *
 * @param pattern 用于生成next数组的字符串
 * @param next next数组
 */
static void makeNext(const string &pattern, vector<nc_int32_t> &next) {
  nc_int32_t pattern_len = pattern.size();

  for (nc_int32_t prefix = 0, suffix = 1; suffix < pattern_len; ++suffix) {
    while (prefix > 0 && pattern[prefix] != pattern[suffix]) {
      prefix = next[prefix - 1];
    }

    if (pattern[prefix] == pattern[suffix]) {
      ++prefix;
    }

    next[suffix] = prefix;
  }
}

nc_int32_t kmp(const string &text, const string &pattern) {
  if (text.empty() || pattern.empty()) {
    return -1;
  }

  // 公共前缀数组
  vector<nc_int32_t> next(pattern.size(), 0);
  makeNext(pattern, next);

  nc_uint32_t text_len = text.size();
  nc_uint32_t text_index = 0;
  nc_uint32_t pattern_len = pattern.size();
  nc_uint32_t pattern_index = 0;

  for (; text_index < text_len; ++text_index) {
    while (pattern_index > 0 && text[text_index] != pattern[pattern_index]) {
      pattern_index = next[pattern_index - 1];
    }

    if (pattern[pattern_index] == text[text_index]) {
      ++pattern_index;
    }

    if (pattern_index == pattern_len) {
      return text_index - pattern_len + 1;
    }
  }

  return -1;
}
