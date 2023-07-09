/**
 * @file my_string.h
 * @author Nick
 * @brief 自定义实现的string类
 * @version 0.1
 * @date 2023-05-17
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef MY_STRING_H_
#define MY_STRING_H_

#include <iostream>

#include "types.h"

class MyString {
  friend MyString operator+(const MyString &lhs, const MyString &rhs);
  friend bool operator==(const MyString &lhs, const MyString &rhs);
  friend bool operator!=(const MyString &lhs, const MyString &rhs);
  friend std::ostream &operator<<(std::ostream &out, const MyString &ms);

 public:
  MyString();
  explicit MyString(const nc_char_t *str);
  MyString(const MyString &ms);
  MyString &operator=(const MyString &rhs);
  ~MyString();

  MyString &operator+=(const MyString &rhs);
  nc_char_t &operator[](size_t index);
  const nc_char_t &operator[](size_t index) const;

  size_t Size() const { return length_; }
  const nc_char_t *toCStr() const { return data_; }
  nc_bool_t Empty() const { return length_ == 0; }

 private:
  nc_char_t *data_;
  size_t length_;
};
MyString operator+(const MyString &lhs, const MyString &rhs);
bool operator==(const MyString &lhs, const MyString &rhs);
bool operator!=(const MyString &lhs, const MyString &rhs);
std::ostream &operator<<(std::ostream &out, const MyString &ms);

#endif  // MY_STRING_H_
