#include "gtest/gtest.h"
#include "MyString.h"

using namespace std;

TEST(myString, init) {
    MyString ms;
    EXPECT_STREQ(ms.c_str(), "");
}

TEST(myString, init2) {
    MyString ms("123");
    EXPECT_STREQ(ms.c_str(), "123");
}

TEST(myString, init3) {
    MyString ms("123");
    MyString ms2(ms);
    EXPECT_STREQ(ms2.c_str(), "123");
}

TEST(myString, equal) {
    MyString ms("123");
    MyString ms2 = ms;
    EXPECT_STREQ(ms2.c_str(), "123");
}

TEST(myString, equalPlus) {
    MyString ms("123");
    MyString ms2("456");
    ms += ms2;
    EXPECT_STREQ(ms.c_str(), "123456");
}

TEST(myString, index) {
    MyString ms("123");
    EXPECT_EQ(ms[0], '1');
}

TEST(myString, index2) {
    const MyString ms("123");
    EXPECT_EQ(ms[0], '1');
}

TEST(myString, size) {
    MyString ms("123");
    EXPECT_EQ(ms.size(), 3);
}

TEST(myString, cstr) {
    MyString ms("123");
    EXPECT_STREQ(ms.c_str(), "123");
}

TEST(myString, empty) {
    MyString notEmptyStr("123");
    EXPECT_FALSE(notEmptyStr.empty());

    MyString emptyStr;
    EXPECT_TRUE(emptyStr.empty());
}

TEST(myString, equalequalAndnotEqual) {
    MyString ms("123");
    MyString ms2("123");
    MyString ms3("345");
    MyString ms4("12");

    EXPECT_TRUE(ms == ms2);
    EXPECT_FALSE(ms == ms3);
    EXPECT_FALSE(ms == ms4);

    EXPECT_FALSE(ms != ms2);
    EXPECT_TRUE(ms != ms3);
    EXPECT_TRUE(ms != ms4);
}

int main(int argc, char **argv) {
    printf("Running main() from %s\n\n", __FILE__);
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
