#include <iostream>

#include "custom_sys_func.h"
#include "gtest/gtest.h"

using namespace std;

TEST(customTest, memcpyCase) {
  nc_char_t str1[5] = {0};
  nc_char_t str2[] = "abc";
  nc_char_t *ret = (nc_char_t *)CustomMemcpy(nullptr, str2, 3);
  EXPECT_STREQ(ret, nullptr);
  ret = (nc_char_t *)CustomMemcpy(str1, nullptr, 3);
  EXPECT_STREQ(ret, nullptr);
  ret = (nc_char_t *)CustomMemcpy(nullptr, nullptr, 3);
  EXPECT_STREQ(ret, nullptr);

  ret = (nc_char_t *)CustomMemcpy(str1, str2, 3);
  EXPECT_STREQ(str1, str2);
}

TEST(customTest, memmoveCase) {
  nc_char_t str1[5] = {0};
  nc_char_t str2[] = "abc";

  nc_char_t *ret = (nc_char_t *)CustomMemmove(nullptr, str2, 3);
  EXPECT_STREQ(ret, nullptr);
  ret = (nc_char_t *)CustomMemmove(str1, nullptr, 3);
  EXPECT_STREQ(ret, nullptr);
  ret = (nc_char_t *)CustomMemmove(nullptr, nullptr, 3);
  EXPECT_STREQ(ret, nullptr);

  ret = (nc_char_t *)CustomMemmove(str1, str2, 3);
  EXPECT_STREQ(str1, str2);
}

TEST(customTest, strcatCase) {
  nc_char_t src[50] = "this is source";
  nc_char_t dest[50] = "this is destination";

  nc_char_t *ret = CustomStrcat(nullptr, src);
  EXPECT_STREQ(ret, nullptr);
  ret = CustomStrcat(dest, nullptr);
  EXPECT_STREQ(ret, nullptr);
  ret = CustomStrcat(nullptr, nullptr);
  EXPECT_STREQ(ret, nullptr);

  ret = CustomStrcat(dest, src);
  EXPECT_STREQ(ret, "this is destinationthis is source");
  EXPECT_STREQ(dest, "this is destinationthis is source");
  EXPECT_STREQ(src, "this is source");
}

TEST(customTest, strcpyCase) {
  nc_char_t src[50] = {0};
  nc_char_t dest[50] = {0};

  nc_char_t *ret = CustomStrcpy(nullptr, src);
  EXPECT_STREQ(ret, nullptr);
  ret = CustomStrcpy(dest, nullptr);
  EXPECT_STREQ(ret, nullptr);
  ret = CustomStrcpy(nullptr, nullptr);
  EXPECT_STREQ(ret, nullptr);

  ret = CustomStrcpy(dest, "this is destination");
  EXPECT_STREQ(ret, "this is destination");
  EXPECT_STREQ(dest, "this is destination");
}

TEST(customTest, strcmpCase) {
  nc_char_t big[50] = "c";
  nc_char_t small[50] = "a";
  nc_char_t small2[50] = "a";

  nc_int32_t ret = CustomStrcmp(big, small);
  EXPECT_EQ(ret, 1);
  ret = CustomStrcmp(small, big);
  EXPECT_EQ(ret, -1);
  ret = CustomStrcmp(small, small2);
  EXPECT_EQ(ret, 0);
}

TEST(customTest, strncmpCase) {
  nc_char_t src[50] = "this is source";
  nc_char_t dest[50] = "this is destination";

  nc_int32_t ret = CustomStrncmp(src, dest, 4);
  EXPECT_EQ(ret, 0);
  ret = CustomStrncmp(src, dest, 0);
  EXPECT_EQ(ret, 0);
  ret = CustomStrncmp(src, dest, 10);
  EXPECT_GT(ret, 0);
  ret = CustomStrncmp(dest, src, 10);
  EXPECT_LT(ret, 0);
}

TEST(customTest, strlenCase) {
  nc_char_t src[50] = "this is source";

  nc_uint32_t ret = CustomStrlen(nullptr);
  EXPECT_EQ(ret, static_cast<nc_uint32_t>(0));
  ret = CustomStrlen(src);
  EXPECT_EQ(ret, static_cast<nc_uint32_t>(14));
}

TEST(customTest, strstrCase) {
  nc_char_t src[50] = "this is source";

  nc_char_t *ret = CustomStrstr(src, nullptr);
  EXPECT_STREQ(ret, nullptr);
  ret = CustomStrstr(src, "");
  EXPECT_STREQ(ret, src);
  ret = CustomStrstr(src, "is");
  EXPECT_STREQ(ret, "is is source");
  ret = CustomStrstr(src, "src");
  EXPECT_STREQ(ret, nullptr);
}

TEST(customTest, atoiCase) {
  nc_char_t str[5] = "1234";
  nc_char_t str2[5] = "abcd";
  nc_char_t str3[] = "2147483649";
  nc_char_t str4[] = "-2147483649";

  nc_int32_t ret = CustomAtoi(str);
  EXPECT_EQ(ret, 1234);
  ret = CustomAtoi(str2);
  EXPECT_EQ(ret, 0);
  ret = CustomAtoi(str3);
  EXPECT_EQ(ret, 0);
  ret = CustomAtoi(str4);
  EXPECT_EQ(ret, 0);
}

TEST(customTest, memmetCase) {
  nc_char_t str[5] = "abcd";

  nc_char_t *ret = (nc_char_t *)CustomMemset(nullptr, 0, 5);
  EXPECT_STRNE(str, nullptr);
  EXPECT_STREQ(ret, nullptr);
  ret = (nc_char_t *)CustomMemset(str, 0, sizeof(str));
  EXPECT_STREQ(ret, "");
  EXPECT_STREQ(str, "");
}

nc_int32_t main(nc_int32_t argc, nc_char_t **argv) {
  printf("Running main() from %s\n\n", __FILE__);
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
