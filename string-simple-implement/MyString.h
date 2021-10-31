#ifndef _MY_STRING_H_
#define _MY_STRING_H_

#include <stddef.h>
#include <iostream>

class MyString {
    friend MyString operator+(const MyString &lhs, const MyString &rhs);
    friend bool operator==(const MyString &lhs, const MyString &rhs);
    friend bool operator!=(const MyString &lhs, const MyString &rhs);
    friend std::ostream &operator<<(std::ostream &out, const MyString &ms);
public:
    MyString();
    MyString(const char *str);
    MyString(const MyString &ms);
    MyString& operator=(const MyString &rhs);
    ~MyString();

    MyString &operator+=(const MyString &rhs);
    char &operator[](size_t index);
    const char &operator[](size_t index) const;

    size_t size() const { return length; }
    const char *c_str() const { return data; }
    bool empty() const { return length == 0; }
private:
    char *data;
    size_t length;
};
MyString operator+(const MyString &lhs, const MyString &rhs);
bool operator==(const MyString &lhs, const MyString &rhs);
bool operator!=(const MyString &lhs, const MyString &rhs);
std::ostream &operator<<(std::ostream &out, const MyString &ms);

#endif
