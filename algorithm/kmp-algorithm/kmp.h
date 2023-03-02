#ifndef _KMP_H_
#define _KMP_H_

#include <string>
#include <vector>

#include "types.h"

/**
 * @brief kmp字符串搜索算法
 * 
 * @param text 需要进行匹配搜索的文本
 * @param pattern 用于匹配的字符串
 * @return nc_int32_t 匹配字符串首字母出现位置的索引, -1表示未找到
 */
nc_int32_t Kmp(const std::string &text, const std::string &pattern);

#endif
