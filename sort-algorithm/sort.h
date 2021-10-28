#ifndef __SORT__H_
#define __SORT__H_

/**
 * @file sort.h
 * @author chen
 * @brief 常见排序算法实现
 * @date 2021-10-28
 * 
 */

#include <vector>

/**
 * @brief 冒泡排序算法
 * 
 * @param data 待排序的数组
 * @return int 返回值，-1表示失败，0表示成功
 */
int bubbleSort(std::vector<int> &data);

/**
 * @brief 选择排序算法
 * 
 * @param data 待排序的数组
 * @return int 返回值，-1表示失败，0表示成功
 */
int selectSort(std::vector<int> &data);

/**
 * @brief 插入排序算法
 * 
 * @param data 待排序的数组
 * @return int 返回值，-1表示失败，0表示成功
 */
int insertSort(std::vector<int> &data);

/**
 * @brief 希尔排序算法
 * 
 * @param data 待排序的数组
 * @return int 返回值，-1表示失败，0表示成功
 */
int shellSort(std::vector<int> &data);

/**
 * @brief 快速排序算法
 * 
 * @param data 待排序的数组
 * @param start 数组的起始位置
 * @param end 数组的末尾位置
 */
void quickSort(std::vector<int> &data, int start, int end);

/**
 * @brief 将数组分成两半，按照大小重新进行排序并合并
 * 
 * @param data 待排序的数组
 * @param start 开始位置
 * @param mid 中间位置
 * @param end 末尾位置
 */
void mergeArray(std::vector<int> &data, int start, int mid, int end);

/**
 * @brief 归并排序算法
 * 
 * @param data 待排序的数组
 * @param start 开始位置
 * @param end 末尾位置
 */
void mergeSort(std::vector<int> &data, int start, int end);

/**
 * @brief 堆排序的下沉调整函数
 * 
 * @param data 待调节的数组
 * @param parent_index 父节点的下标
 * @param len 排序长度
 */
void downAdjust(std::vector<int> &data, int parent_index, int len);

/**
 * @brief 堆排序算法
 * 
 * @param data 待排序的数组
 */
void heapSort(std::vector<int> &data);

#endif