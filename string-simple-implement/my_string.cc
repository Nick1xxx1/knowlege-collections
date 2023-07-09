#include "my_string.h"

#include <assert.h>
#include <string.h>

#include <exception>

namespace {
  constexpr nc_uint8_t kDefaultStrSize = 1;
}

MyString::MyString() {
  data_ = new (std::nothrow) nc_char_t[kDefaultStrSize];
  assert(data_);

  *data_ = '\0';
  length_ = 0;
}

MyString::MyString(const nc_char_t *str) {
  assert(str);

  length_ = strlen(str);
  data_ = new (std::nothrow) nc_char_t[length_ + 1];
  assert(data_);
  memcpy(data_, str, length_ + 1);
}

MyString::MyString(const MyString &ms) {
  length_ = ms.Size();
  data_ = new (std::nothrow) nc_char_t[length_ + 1];
  assert(data_);
  memcpy(data_, ms.toCStr(), length_ + 1);
}

MyString::~MyString() {
  if (data_) {
    delete[] data_;
    data_ = nullptr;
  }

  length_ = 0;
}

MyString &MyString::operator=(const MyString &rhs) {
  if (this == &rhs) {
    return *this;
  }

  delete[] data_;
  length_ = rhs.Size();
  data_ = new (std::nothrow) nc_char_t[length_ + 1];
  assert(data_);
  memcpy(data_, rhs.toCStr(), length_ + 1);
  return *this;
}

MyString &MyString::operator+=(const MyString &rhs) {
  size_t old_len = length_;
  length_ += rhs.Size();
  nc_char_t *tmp_data = new (std::nothrow) nc_char_t[length_ + 1];
  assert(tmp_data);
  memcpy(tmp_data, data_, old_len);
  memcpy(tmp_data + old_len, rhs.toCStr(), rhs.Size() + 1);
  delete[] data_;
  data_ = tmp_data;

  return *this;
}

nc_char_t &MyString::operator[](size_t index) {
  if (index >= length_) {
    throw std::out_of_range("out of range");
  }

  return const_cast<nc_char_t &>(static_cast<const MyString &>(*this)[index]);
}

const nc_char_t &MyString::operator[](size_t index) const {
  if (index >= length_) {
    throw std::out_of_range("out of range");
  }

  return data_[index];
}

MyString operator+(const MyString &lhs, const MyString &rhs) {
  MyString tmp(lhs);
  tmp += rhs;
  return tmp;
}

bool operator==(const MyString &lhs, const MyString &rhs) {
  if (lhs.Size() != rhs.Size()) {
    return false;
  }

  return strcmp(lhs.toCStr(), rhs.toCStr()) == 0;
}

bool operator!=(const MyString &lhs, const MyString &rhs) {
  return !(lhs == rhs);
}

std::ostream &operator<<(std::ostream &out, const MyString &ms) {
  out << ms.toCStr();
  return out;
}
