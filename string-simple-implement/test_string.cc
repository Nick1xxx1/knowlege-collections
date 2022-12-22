#include "gtest/gtest.h"
#include "my_string.h"

using namespace std;

TEST(myString, Init) {
  MyString ms;
  EXPECT_STREQ(ms.toCStr(), "");
}

TEST(myString, Init2) {
  MyString ms("123");
  EXPECT_STREQ(ms.toCStr(), "123");
}

TEST(myString, Init3) {
  MyString ms("123");
  MyString ms2(ms);
  EXPECT_STREQ(ms2.toCStr(), "123");
}

TEST(myString, Equal) {
  MyString ms("123");
  MyString ms2 = ms;
  EXPECT_STREQ(ms2.toCStr(), "123");
}

TEST(myString, EqualPlus) {
  MyString ms("123");
  MyString ms2("456");
  ms += ms2;
  EXPECT_STREQ(ms.toCStr(), "123456");
}

TEST(myString, Index) {
  MyString ms("123");
  EXPECT_EQ(ms[0], '1');
}

TEST(myString, Index2) {
  const MyString ms("123");
  EXPECT_EQ(ms[0], '1');
}

TEST(myString, Size) {
  MyString ms("123");
  EXPECT_EQ(ms.Size(), static_cast<size_t>(3));
}

TEST(myString, ToCStr) {
  MyString ms("123");
  EXPECT_STREQ(ms.toCStr(), "123");
}

TEST(myString, Empty) {
  MyString notEmptyStr("123");
  EXPECT_FALSE(notEmptyStr.Empty());

  MyString emptyStr;
  EXPECT_TRUE(emptyStr.Empty());
}

TEST(myString, EqualequalAndnotEqual) {
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
