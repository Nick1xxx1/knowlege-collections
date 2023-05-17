/**
 * @file bloom_filter.h
 * @author Nick
 * @brief 布隆过滤器实现
 * @version 0.1
 * @date 2023-05-17
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef BLOOM_FILTER_H_
#define BLOOM_FILTER_H_

#include <cmath>
#include <memory>
#include <stdexcept>

#include "types.h"

class BloomFilter {
 public:
  explicit BloomFilter(nc_uint32_t seed, nc_uint32_t max_item_num,
                       nc_float64_t positive_false)
      : max_item_num_(max_item_num),
        positive_false_(positive_false),
        seed_(seed) {
    if (positive_false_ <= kElipse || positive_false_ >= 1.0) {
      throw std::out_of_range("Positive false should range in (0, 1)");
    }

    ResetBloomFilter();
  }

  ~BloomFilter() {}

  BloomFilter() = delete;
  // 禁止拷贝和赋值
  BloomFilter(const BloomFilter &) = delete;
  BloomFilter &operator=(const BloomFilter &) = delete;

  /**
   * @brief 设置用于计算哈希函数的种子
   *
   * @param seed 设置的种子
   */
  void SetSeed(nc_uint32_t seed) { seed_ = seed; }

  /**
   * @brief 设置布隆过滤器期望的最大元素数量, 设置后需要重新计算m和k
   *
   * @param max_item_num 设置的最大元素数量
   */
  void SetMaxItemNum(nc_uint32_t max_item_num) {
    max_item_num_ = max_item_num;

    if (positive_false_ < kElipse) {
      // 假阳率未设置, 不进行计算
      return;
    }
    ResetBloomFilter();
  }

  /**
   * @brief 设置布隆过滤器期望的假阳率, 设置后需要重新计算m和k
   *
   * @param positive_false 设置的假阳率
   */
  void SetPositiveFalse(nc_float64_t positive_false) {
    if (positive_false <= kElipse || positive_false_ >= 1.0) {
      throw std::out_of_range("Positive false should range in (0, 1)");
    }

    positive_false_ = positive_false;

    if (max_item_num_ == 0) {
      // 元素数量未设置, 不进行计算
      return;
    }
    ResetBloomFilter();
  }

  void Reset(nc_uint32_t seed, nc_uint32_t max_item_num,
             nc_float64_t positive_false) {
    if (positive_false_ <= kElipse || positive_false_ >= 1.0) {
      throw std::out_of_range("Positive false should range in (0, 1)");
    }

    seed_ = seed;
    max_item_num = max_item_num_;
    positive_false_ = positive_false;

    ResetBloomFilter();
  }

  /**
   * @brief 向布隆过滤器中添加元素
   *
   * @param key 待添加的值
   * @param len 值的长度
   * @return Result 添加结果
   */
  Result AddValue(const void *key, nc_uint32_t len);

  /**
   * @brief 查询某个值是否存在于布隆过滤器中
   *
   * @param key 待查询的值
   * @param len 值的长度
   * @return Result 查询结果
   */
  Result CheckValue(const void *key, nc_uint32_t len);

  /**
   * @brief 调试用函数, 用于查看布隆过滤器中各个参数的值
   *
   */
  void PrintBloomFilterInfo();

 private:
  /**
   * @brief 计算布隆过滤器中的参数, 根据指定的n和p计算m和k
   *
   */
  void CalBloomFilterParams() {
    // 根据最大元素数量和假阳率计算布隆过滤器的位图大小
    filter_bits_ = static_cast<nc_uint32_t>(
        ceil((max_item_num_ * log(positive_false_)) / -log(2.0) * log(2.0)));
    // 对齐8字节
    const nc_uint32_t align_bits = 8U * kByteBits;
    filter_bits_ = (filter_bits_ - filter_bits_ % align_bits) + align_bits;
    filter_size_ = filter_bits_ / kByteBits;

    // 根据最大元素数量和位图大小计算哈希函数数量
    hash_funcs_num_ = static_cast<nc_uint32_t>(
        round(log(2.0) * filter_bits_ / max_item_num_));
  }

  /**
   * @brief 重置布隆过滤器
   *
   */
  void ResetBloomFilter() {
    CalBloomFilterParams();

    bitmap_.reset(new nc_uint8_t[filter_size_]);
    hash_funcs_.reset(new nc_uint32_t[hash_funcs_num_]);
    item_count_ = 0;
  }

  void CalHash(const void *key, nc_uint32_t len);

  void SetBit(nc_uint32_t n) {
    bitmap_[n / kByteBits] |= (1 << (n % kByteBits));
  }

  uint32_t GetBit(nc_uint32_t n) {
    return bitmap_[n / kByteBits] & (1 << (n % kByteBits));
  }

 private:
  nc_uint32_t max_item_num_;     // n - BloomFilter中元素的最大数量
  nc_float64_t positive_false_;  // p - 假阳率, 范围为0-1
  nc_uint32_t
      filter_bits_;  // m - BloomFilter的位图的大小
                     // 计算公式: ceil((n * log(p)) / log(1 / pow(2, log(2))))
  nc_uint32_t hash_funcs_num_;  // k - 哈希函数的个数
                                // 计算公式: round(log(2.0) * m / n)

  nc_uint32_t seed_;        // murmurhash的种子偏移量
  nc_uint32_t item_count_;  // 当前元素的数量

  nc_uint32_t filter_size_;  // BloomFilter的字节数, filter_bits_ / 8

  std::unique_ptr<nc_uint8_t[]> bitmap_;    // BloomFilter中的数据
  std::unique_ptr<uint32_t[]> hash_funcs_;  // 哈希函数数组
};

#endif  // BLOOM_FILTER_H_
