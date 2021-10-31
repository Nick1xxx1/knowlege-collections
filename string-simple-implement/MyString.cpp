#include "MyString.h"
#include <assert.h>
#include <string.h>
#include <exception>
using namespace std;

#define DEFAULT_STRING_SIZE 1

MyString::MyString() {
    data = new (std::nothrow) char[DEFAULT_STRING_SIZE];
    assert(data);

    *data = '\0';
    length = 0;
}

MyString::MyString(const char *str) {
    assert(str);

    length = strlen(str);
    data = new (std::nothrow) char[length + 1];
    assert(data);
    memcpy(data, str, length + 1);
}

MyString::MyString(const MyString &ms) {
    length = ms.size();
    data = new (std::nothrow) char[length + 1];
    assert(data);
    memcpy(data, ms.c_str(), length + 1);
}

MyString::~MyString() {
    if (data) {
        delete[] data;
        data = nullptr;
    }

    length = 0;
}

MyString& MyString::operator=(const MyString &rhs) {
    if (this == &rhs) {
        return *this;
    }

    delete[] data;
    length = rhs.size();
    data = new (std::nothrow) char[length + 1];
    assert(data);
    memcpy(data, rhs.c_str(), length + 1);
    return *this;
}

MyString& MyString::operator+=(const MyString &rhs) {
    size_t oldLen = length;
    length += rhs.size();
    char *tmpData = new (std::nothrow) char[length + 1];
    assert(tmpData);
    memcpy(tmpData, data, oldLen);
    memcpy(tmpData + oldLen, rhs.c_str(), rhs.size() + 1);
    delete[] data;
    data = tmpData;

    return *this;
}

char& MyString::operator[](size_t index) {
    if (index >= length) {
        throw out_of_range("out of range");
    }

    return const_cast<char&>(static_cast<const MyString&>(*this)[index]);
}

const char& MyString::operator[](size_t index) const {
    if (index >= length) {
        throw out_of_range("out of range");
    }

    return data[index];
}

MyString operator+(const MyString &lhs, const MyString &rhs) {
    MyString tmp(lhs);
    tmp += rhs;
    return tmp;
}

bool operator==(const MyString &lhs, const MyString &rhs) {
    if (lhs.size() != rhs.size()) {
        return false;
    }

    return strcmp(lhs.c_str(), rhs.c_str()) == 0;
}

bool operator!=(const MyString &lhs, const MyString &rhs) {
    return !(lhs == rhs);
}

std::ostream &operator<<(std::ostream &out, const MyString &ms) {
    out << ms.c_str();
    return out;
}
