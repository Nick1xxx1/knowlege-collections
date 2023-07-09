/**
 * @file sort.h
 * @author Nick
 * @brief 常见排序算法实现
 * @version 0.1
 * @date 2023-05-17
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef SORT_H_
#define SORT_H_

#include <utility>
#include <vector>

#include "types.h"

/**
 * @brief 冒泡排序
 * 
 * @tparam T 待排序的数组中变量的类型
 * @param sort_data 待排序的数组
 * @return Result 排序结果, kError表示失败, kOk表示成功
 */
template<typename T>
Result BubbleSort(std::vector<T> &sort_data) {
  if (sort_data.empty()) {
    return Result::kError;
  }

  size_t data_len = sort_data.size();
  for (size_t i = 0; i < data_len - 1; ++i) {
    bool exchanged = false;
    for (size_t j = 0; j < data_len - 1 - i; ++j) {
      if (sort_data[j] > sort_data[j + 1]) {
        using std::swap;
        swap(sort_data[j], sort_data[j + 1]);
        exchanged = true;
      }
    }

    if (!exchanged) {
      break;
    }
  }

  return Result::kOk;
}

/**
 * @brief 选择排序
 * 
 * @tparam T 待排序的数组中变量的类型
 * @param sort_data 待排序的数组
 * @return Result 排序结果, kError表示失败, kOk表示成功
 */
template<typename T>
Result SelectSort(std::vector<T> &sort_data) {
  if (sort_data.empty()) {
    return Result::kError;
  }

  size_t data_len = sort_data.size();
  for (size_t i = 0; i < data_len - 1; ++i) {
    for (size_t j = i + 1; j < data_len; ++j) {
      if (sort_data[i] > sort_data[j]) {
        using std::swap;
        swap(sort_data[i], sort_data[j]);
      }
    }
  }

  return Result::kOk;
}

/**
 * @brief 插入排序
 * 
 * @tparam T 待排序的数组中变量的类型
 * @param sort_data 待排序的数组
 * @return Result 排序结果, kError表示失败, kOk表示成功
 */
template<typename T>
Result InsertSort(std::vector<T> &sort_data) {
  if (sort_data.empty()) {
    return Result::kError;
  }

  size_t data_len = sort_data.size();
  for (size_t i = 1; i < data_len; ++i) {
    size_t j = i - 1;
    T tmp_data = sort_data[i];
    for (; j >= 0 && tmp_data < sort_data[j]; --j) {
      sort_data[j + 1] = sort_data[j];
    }

    sort_data[j + 1] = tmp_data;
  }

  return Result::kOk;
}

/**
 * @brief 希尔排序
 * 
 * @tparam T 待排序的数组中变量的类型
 * @param sort_data 待排序的数组
 * @return Result 排序结果, kError表示失败, kOk表示成功
 */
template<typename T>
Result ShellSort(std::vector<T> &sort_data) {
  if (sort_data.empty()) {
    return Result::kError;
  }

  nc_int32_t data_len = sort_data.size();
  for (nc_int32_t step = data_len / 2; step >= 1; step /= 2) {
    for (nc_int32_t i = step; i < data_len; ++i) {
      nc_int32_t j = i - step;
      T tmp_data = sort_data[i];
      for (; j >= 0 && tmp_data < sort_data[j]; j -= step) {
        sort_data[j + step] = sort_data[j];
      }

      sort_data[j + step] = tmp_data;
    }
  }

  return Result::kOk;
}

/**
 * @brief 分组辅助函数
 * 
 * @tparam T 待排序的数组中变量的类型
 * @param sort_data 待排序的数组
 * @param start 起始索引
 * @param end 终止索引
 * @return nc_int32_t 起始哨兵最后所在位置的索引
 */
template<typename T>
inline nc_int32_t PartitionArray(std::vector<T> &sort_data, nc_int32_t start, nc_int32_t end) {
  nc_int32_t left = start;
  nc_int32_t right = end;
  T pivot = sort_data[start];
  nc_int32_t index = start;  // 哨兵的初始位置

  while (left < right) {
    while (left < right) {
      // 如果右边的值比哨兵的值大，则继续从右找
      if (pivot < sort_data[right]) {
        --right;
        continue;
      }
      // 找到比哨兵更小的值，则进行值替换，并记录此时哨兵的位置
      // 然后跳出循环
      sort_data[index] = sort_data[right];
      index = right;
      ++left;
      break;
    }

    while (left < right) {
      if (pivot >= sort_data[left]) {
        ++left;
        continue;
      }

      sort_data[index] = sort_data[left];
      index = left;
      --right;
      break;
    }
  }

  // 将哨兵的值重新赋值回去
  sort_data[index] = pivot;
  // 返回哨兵的位置
  return index;
}

/**
 * @brief 快速排序
 * 
 * @tparam T 待排序的数组中变量的类型
 * @param sort_data 待排序的数组
 * @param start 起始索引
 * @param end 终止索引
 */
template<typename T>
void QuickSort(std::vector<T> &sort_data, nc_int32_t start, nc_int32_t end) {
  if (start >= end) {
    return;
  }

  int pivotIndex = PartitionArray(sort_data, start, end);
  QuickSort(sort_data, start, pivotIndex - 1);
  QuickSort(sort_data, pivotIndex + 1, end);
}

/**
 * @brief 分组辅助函数
 * 
 * @tparam T 待排序的数组中变量的类型
 * @param sort_data 待排序的数组
 * @param start 起始索引
 * @param mid 中间索引
 * @param end 终止索引
 */
template<typename T>
inline void MergeArray(std::vector<T> &sort_data, nc_int32_t start, nc_int32_t mid, nc_int32_t end) {
  std::vector<T> tmp_vec(end - start + 1);
  nc_int32_t pos1 = start;
  nc_int32_t pos2 = mid + 1;
  nc_int32_t pos = 0;

  while (pos1 <= mid && pos2 <= end) {
    if (sort_data[pos1] <= sort_data[pos2]) {
      tmp_vec[pos++] = sort_data[pos1++];
    } else {
      tmp_vec[pos++] = sort_data[pos2++];
    }
  }

  while (pos1 <= mid) {
    tmp_vec[pos++] = sort_data[pos1++];
  }

  while (pos2 <= end) {
    tmp_vec[pos++] = sort_data[pos2++];
  }

  for (int i = 0; i < end - start + 1; ++i) {
    sort_data[start + i] = tmp_vec[i];
  }
}

/**
 * @brief 归并排序
 * 
 * @tparam T 待排序的数组中变量的类型
 * @param sort_data 待排序的数组
 * @param start 起始索引
 * @param end 终止索引
 */
template<typename T>
void MergeSort(std::vector<T> &sort_data, nc_int32_t start, nc_int32_t end) {
  if (start >= end) {
    return;
  }

  nc_int32_t mid = start + (end - start) / 2;
  MergeSort(sort_data, start, mid);
  MergeSort(sort_data, mid + 1, end);
  MergeArray(sort_data, start, mid, end);
}

/**
 * @brief 下沉调节函数, 形成最大堆
 * 
 * @tparam T 待排序的数组中变量的类型
 * @param sort_data 待排序的数组
 * @param parent_index 父节点索引
 * @param len 数组长度
 */
template <typename T>
inline void DownAdjust(std::vector<T> &sort_data, nc_int32_t parent_index,
                       nc_int32_t len) {
  T parentValue = sort_data[parent_index];
  nc_int32_t childIndex = parent_index * 2 + 1;
  while (childIndex < len) {
    if (childIndex + 1 < len && sort_data[childIndex + 1] > sort_data[childIndex]) {
      childIndex++;
    }

    if (parentValue >= sort_data[childIndex]) {
      break;
    }

    sort_data[parent_index] = sort_data[childIndex];
    parent_index = childIndex;
    childIndex = parent_index * 2 + 1;
  }

  sort_data[parent_index] = parentValue;
}

/**
 * @brief 堆排序
 * 
 * @tparam T 待排序的数组中变量的类型
 * @param sort_data 待排序的数组
 */
template <typename T>
void HeapSort(std::vector<T> &sort_data) {
  nc_int32_t len = sort_data.size();

  // 构造大根堆
  for (nc_int32_t i = len / 2 - 1; i >= 0; --i) {
    DownAdjust(sort_data, i, len);
  }

  for (nc_int32_t i = len - 1; i > 0; --i) {
    using std::swap;
    swap(sort_data[i], sort_data[0]);

    DownAdjust(sort_data, 0, i);
  }
}

#endif // SORT_H_
