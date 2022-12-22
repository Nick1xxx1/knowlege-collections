#ifndef _MY_STRING_H_
#define _MY_STRING_H_

#include <iostream>

#include "types.h"

class MyString {
  friend MyString operator+(const MyString &lhs, const MyString &rhs);
  friend bool operator==(const MyString &lhs, const MyString &rhs);
  friend bool operator!=(const MyString &lhs, const MyString &rhs);
  friend std::ostream &operator<<(std::ostream &out, const MyString &ms);
public:
  MyString();
  MyString(const nc_char_t *str);
  MyString(const MyString &ms);
  MyString& operator=(const MyString &rhs);
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

#endif
