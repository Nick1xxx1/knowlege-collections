#ifndef _KMP_H_
#define _KMP_H_

/**
 * @brief kmp算法
 * 
 * @param text 待匹配的文本字符串
 * @param pattern 要查找的内容
 * @param next 前缀和后缀最大公共步长的数组
 * @return int 带查找内容的位置
 */
int kmp(const char *text, const char *pattern, int *next);

#endif
