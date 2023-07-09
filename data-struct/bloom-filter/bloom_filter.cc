#include "bloom_filter.h"

#include <cmath>
#include <iostream>

using std::cout;
using std::endl;

namespace {
// MurmurHash2, 64-bit versions, by Austin Appleby
// https://sites.google.com/site/murmurhash/
inline nc_uint64_t MurmurHash2(const void *key, int len, uint32_t seed) {
  const nc_uint64_t m = 0xc6a4a7935bd1e995;
  const int r = 47;

  nc_uint64_t h = seed ^ (len * m);

  const nc_uint64_t *data = (const nc_uint64_t *)key;
  const nc_uint64_t *end = data + (len / 8);

  while (data != end) {
    nc_uint64_t k = *data++;

    k *= m;
    k ^= k >> r;
    k *= m;

    h ^= k;
    h *= m;
  }

  const uint8_t *data2 = (const uint8_t *)data;

  switch (len & 7) {
    case 7:
      h ^= ((nc_uint64_t)data2[6]) << 48;
    case 6:
      h ^= ((nc_uint64_t)data2[5]) << 40;
    case 5:
      h ^= ((nc_uint64_t)data2[4]) << 32;
    case 4:
      h ^= ((nc_uint64_t)data2[3]) << 24;
    case 3:
      h ^= ((nc_uint64_t)data2[2]) << 16;
    case 2:
      h ^= ((nc_uint64_t)data2[1]) << 8;
    case 1:
      h ^= ((nc_uint64_t)data2[0]);
      h *= m;
  }

  h ^= h >> r;
  h *= m;
  h ^= h >> r;

  return h;
}

inline nc_uint32_t MixUint64(nc_uint64_t v) {
  return static_cast<nc_uint32_t>((v >> 32) ^ v);
}

}  // namespace

Result BloomFilter::AddValue(const void *key, nc_uint32_t len) {
  if (!key) {
    return Result::kError;
  }

  CalHash(key, len);
  // 将计算出的hash值设置到位图中
  for (size_t i = 0; i < hash_funcs_num_; ++i) {
    SetBit(hash_funcs_[i]);
  }

  ++item_count_;

  return Result::kOk;
}

Result BloomFilter::CheckValue(const void *key, nc_uint32_t len) {
  if (!key) {
    return Result::kError;
  }

  CalHash(key, len);
  // 判断所有计算出来的hash值是否都在位图中
  for (size_t i = 0; i < hash_funcs_num_; ++i) {
    if (GetBit(hash_funcs_[i]) == 0) {
      return Result::kNotExist;
    }
  }

  return Result::kExist;
}

void BloomFilter::PrintBloomFilterInfo() {
  cout << "Bloom filter information:" << endl
       << "n = " << max_item_num_ << ", p = " << positive_false_
       << ", m = " << filter_bits_ << ", k = " << hash_funcs_num_ << endl
       << endl;
}

void BloomFilter::CalHash(const void *key, nc_uint32_t len) {
  nc_uint64_t hash1 = MurmurHash2(key, len, seed_);
  nc_uint64_t hash2 = MurmurHash2(key, len, MixUint64(hash1));

  for (size_t i = 0; i < hash_funcs_num_; ++i) {
    hash_funcs_[i] = (hash1 + i * hash2) % filter_bits_;
  }
}
