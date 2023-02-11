#ifndef BLOOM_FILTER_H_
#define BLOOM_FILTER_H_

#include <cmath>
#include <stdexcept>
#include <memory>

#include "types.h"

class BloomFilter {
  public:
    BloomFilter()
      : max_item_num_(0), positive_false_(0), filter_bits_(0), hash_funcs_num_(0), seed_(0),
        item_count_(0), filter_size_(0), data_(nullptr), hash_funcs_(nullptr) {}
    explicit BloomFilter(nc_uint32_t seed, nc_uint32_t max_item_num,
                         nc_float64_t positive_false)
        : max_item_num_(max_item_num),
          positive_false_(positive_false),
          seed_(seed),
          item_count_(0) {
      if (positive_false_ <= 0 || positive_false_ >= 1) {
        throw std::out_of_range("Positive false should range in (0, 1)");
      }
      InitBloomFilter();
    }

    ~BloomFilter() {}

    // 禁止拷贝和赋值
    BloomFilter(const BloomFilter&) = delete;
    BloomFilter &operator=(const BloomFilter&) = delete;

    /**
     * @brief 设置用于计算哈希函数的种子
     * 
     * @param seed 设置的种子
     */
    void SetSeed(nc_uint32_t seed) {
      seed_ = seed;
    }

    /**
     * @brief 设置布隆过滤器期望的最大元素数量, 设置后需要重新计算m和k
     * 
     * @param max_item_num 设置的最大元素数量
     */
    void SetMaxItemNum(nc_uint32_t max_item_num) {
      max_item_num_ = max_item_num;
      CalBloomFilterParams();
    }

    /**
     * @brief 设置布隆过滤器期望的假阳率, 设置后需要重新计算m和k
     * 
     * @param positive_false 设置的假阳率
     */
    void SetPositiveFalse(nc_float64_t positive_false) {
      positive_false_ = positive_false;
      CalBloomFilterParams();
    }

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
     * @brief 初始化布隆过滤器
     * 
     */
    void InitBloomFilter() {
      CalBloomFilterParams();

      data_.reset(new nc_uint8_t[filter_size_]);
      hash_funcs_.reset(new nc_uint32_t[hash_funcs_num_]);
    }

  private:
    nc_uint32_t max_item_num_;    // n - BloomFilter中元素的最大数量
    nc_float64_t positive_false_; // p - 假阳率, 范围为0-1
    nc_uint32_t filter_bits_;     // m - BloomFilter的位图的大小
                                  // 计算公式: ceil((n * log(p)) / log(1 / pow(2, log(2))))
    nc_uint32_t hash_funcs_num_;  // k - 哈希函数的个数
                                  // 计算公式: round(log(2.0) * m / n)

    nc_uint32_t seed_;            // murmurhash的种子偏移量
    nc_uint32_t item_count_;      // 当前元素的数量
    
    nc_uint32_t filter_size_;     // BloomFilter的字节数, filter_bits_ / 8

    std::unique_ptr<nc_uint8_t> data_;      // BloomFilter中的数据 
    std::unique_ptr<uint32_t> hash_funcs_;  // 哈希函数数组
};

#endif // BLOOM_FILTER_H_
