#ifndef _CUSTOM_SYS_
#define _CUSTOM_SYS_

#include "stddef.h"

/**
 * @brief 内存拷贝函数
 * 
 * @param dest 目标指针
 * @param src 源指针
 * @param n 待拷贝的字节数
 * @return void* 指向目标指针
 */
void *customMemcpy(void *dest, void *src, size_t n);

/**
 * @brief 内存拷贝函数，能够判断内存重叠的情况
 * 
 * @param dest 目标指针
 * @param src 源指针
 * @param n 待拷贝的字节数
 * @return void* 指向目标指针
 */
void *customMemmove(void *dest, void *src, size_t n);

/**
 * @brief 字符串追加函数
 * 
 * @param dst 目标字符串
 * @param src 源字符串
 * @return char* 指向目标指针
 */
char *customStrcat(char *dest, const char *src);

/**
 * @brief 字符串拷贝函数
 * 
 * @param dst 目标字符串
 * @param src 源字符串
 * @return char* 指向目标指针
 */
char *customStrcpy(char *dest, const char *src);

/**
 * @brief 字符串比较函数
 * 
 * @param s1 待比较的字符串
 * @param s2 待比较的字符串
 * @return int 0表示相等，1表示s1>s2，-1表示s2>s1
 */
int customStrcmp(const char *s1, const char *s2);

/**
 * @brief 字符串比较函数，可传入比较的位数
 * 
 * @param s1 待比较的字符串
 * @param s2 待比较的字符串
 * @param n 比较的位数
 * @return int 0表示相等，1表示s1>s2，-1表示s2>s1
 */
int customStrncmp(const char *s1, const char *s2, size_t n);

/**
 * @brief 字符串长度计算函数
 * 
 * @param str 待计算的字符串
 * @return size_t 字符串长度
 */
size_t customStrlen(const char *str);

/**
 * @brief 字符串查找函数
 * 
 * @param s1 待搜索的字符串
 * @param s2 需要搜索的字符串
 * @return char* 字符串的位置
 */
char *customStrstr(const char *s1, const char *s2);

/**
 * @brief 字符串转整数
 * 
 * @param str 待转换的字符串
 * @return int 转换后的数字
 */
int customAtoi(char *str);

/**
 * @brief 内存设置函数
 * 
 * @param s 待设置的内存
 * @param c 要设置成的值
 * @param n 内存长度
 * @return void* 指向待设置的内存
 */
void *customMemset(void *s, int c, size_t n);

#endif
