#include "bloom_filter.h"

#include <cmath>
#include <iostream>

using std::cout;
using std::endl;

void BloomFilter::PrintBloomFilterInfo() {
  cout << "Bloom filter information:" << endl
       << "n = " << max_item_num_ << ", p = " << positive_false_
       << ", m = " << filter_bits_ << ", k = " << hash_funcs_num_ << endl << endl;
}
