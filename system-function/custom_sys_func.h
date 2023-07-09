/**
 * @file custom_sys_func.h
 * @author Nick
 * @brief 自定义实现的系统函数
 * @version 0.1
 * @date 2023-05-17
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef CUSTOM_SYS_FUNC_H_
#define CUSTOM_SYS_FUNC_H_

#include <stddef.h>

#include "types.h"

/**
 * @brief 内存拷贝函数
 * 
 * @param dest 目标指针
 * @param src 源指针
 * @param n 待拷贝的字节数
 * @return void* 指向目标指针
 */
void *CustomMemcpy(void *dest, void *src, nc_uint32_t n);

/**
 * @brief 内存拷贝函数，能够判断内存重叠的情况
 * 
 * @param dest 目标指针
 * @param src 源指针
 * @param n 待拷贝的字节数
 * @return void* 指向目标指针
 */
void *CustomMemmove(void *dest, void *src, nc_uint32_t n);

/**
 * @brief 字符串追加函数
 * 
 * @param dst 目标字符串
 * @param src 源字符串
 * @return nc_char_t* 指向目标指针
 */
nc_char_t *CustomStrcat(nc_char_t *dest, const nc_char_t *src);

/**
 * @brief 字符串拷贝函数
 * 
 * @param dst 目标字符串
 * @param src 源字符串
 * @return nc_char_t* 指向目标指针
 */
nc_char_t *CustomStrcpy(nc_char_t *dest, const nc_char_t *src);

/**
 * @brief 字符串比较函数
 * 
 * @param s1 待比较的字符串
 * @param s2 待比较的字符串
 * @return nc_int32_t 0表示相等，1表示s1>s2，-1表示s2>s1
 */
nc_int32_t CustomStrcmp(const nc_char_t *s1, const nc_char_t *s2);

/**
 * @brief 字符串比较函数，可传入比较的位数
 * 
 * @param s1 待比较的字符串
 * @param s2 待比较的字符串
 * @param n 比较的位数
 * @return nc_int32_t 0表示相等，1表示s1>s2，-1表示s2>s1
 */
nc_int32_t CustomStrncmp(const nc_char_t *s1, const nc_char_t *s2, nc_uint32_t n);

/**
 * @brief 字符串长度计算函数
 * 
 * @param str 待计算的字符串
 * @return nc_uint32_t 字符串长度
 */
nc_uint32_t CustomStrlen(const nc_char_t *str);

/**
 * @brief 字符串查找函数
 * 
 * @param s1 待搜索的字符串
 * @param s2 需要搜索的字符串
 * @return nc_char_t* 字符串的位置
 */
nc_char_t *CustomStrstr(const nc_char_t *s1, const nc_char_t *s2);

/**
 * @brief 字符串转整数
 * 
 * @param str 待转换的字符串
 * @return nc_int32_t 转换后的数字
 */
nc_int32_t CustomAtoi(nc_char_t *str);

/**
 * @brief 内存设置函数
 * 
 * @param mem 待设置的内存
 * @param val 要设置成的值
 * @param n 内存长度
 * @return void* 指向待设置的内存
 */
void *CustomMemset(void *mem, nc_int32_t val, nc_uint32_t n);

#endif // CUSTOM_SYS_FUNC_H_
