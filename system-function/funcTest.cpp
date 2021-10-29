extern "C"
{
#include "customSys.h"
}
#include "gtest/gtest.h"

#include <iostream>

using namespace std;

TEST(customTest, memcpyCase)
{
    char str1[5] = {0};
    char str2[] = "abc";
    char *ret = (char *)customMemcpy(NULL, str2, 3);
    EXPECT_STREQ(ret, NULL);
    ret = (char *)customMemcpy(str1, NULL, 3);
    EXPECT_STREQ(ret, NULL);
    ret = (char *)customMemcpy(NULL, NULL, 3);
    EXPECT_STREQ(ret, NULL);

    ret = (char *)customMemcpy(str1, str2, 3);
    EXPECT_STREQ(str1, str2);
}

TEST(customTest, memmoveCase)
{
    char str1[5] = {0};
    char str2[] = "abc";

    char *ret = (char *)customMemmove(NULL, str2, 3);
    EXPECT_STREQ(ret, NULL);
    ret = (char *)customMemmove(str1, NULL, 3);
    EXPECT_STREQ(ret, NULL);
    ret = (char *)customMemmove(NULL, NULL, 3);
    EXPECT_STREQ(ret, NULL);

    ret = (char *)customMemmove(str1, str2, 3);
    EXPECT_STREQ(str1, str2);
}

TEST(customTest, strcatCase)
{
    char src[50] = "this is source";
    char dest[50] = "this is destination";

    char *ret = customStrcat(NULL, src);
    EXPECT_STREQ(ret, NULL);
    ret = customStrcat(dest, NULL);
    EXPECT_STREQ(ret, NULL);
    ret = customStrcat(NULL, NULL);
    EXPECT_STREQ(ret, NULL);

    ret = customStrcat(dest, src);
    EXPECT_STREQ(ret, "this is destinationthis is source");
    EXPECT_STREQ(dest, "this is destinationthis is source");
    EXPECT_STREQ(src, "this is source");
}

TEST(customTest, strcpyCase)
{
    char src[50] = {0};
    char dest[50] = {0};

    char *ret = customStrcpy(NULL, src);
    EXPECT_STREQ(ret, NULL);
    ret = customStrcpy(dest, NULL);
    EXPECT_STREQ(ret, NULL);
    ret = customStrcpy(NULL, NULL);
    EXPECT_STREQ(ret, NULL);

    ret = customStrcpy(dest, "this is destination");
    EXPECT_STREQ(ret, "this is destination");
    EXPECT_STREQ(dest, "this is destination");
}

TEST(customTest, strcmpCase)
{
    char big[50] = "c";
    char small[50] = "a";
    char small2[50] = "a";

    int ret = customStrcmp(big, small);
    EXPECT_EQ(ret, 1);
    ret = customStrcmp(small, big);
    EXPECT_EQ(ret, -1);
    ret = customStrcmp(small, small2);
    EXPECT_EQ(ret, 0);
}

TEST(customTest, strncmpCase)
{
    char src[50] = "this is source";
    char dest[50] = "this is destination";

    int ret = customStrncmp(src, dest, 4);
    EXPECT_EQ(ret, 0);
    ret = customStrncmp(src, dest, 0);
    EXPECT_EQ(ret, 0);
    ret = customStrncmp(src, dest, 10);
    EXPECT_GT(ret, 0);
    ret = customStrncmp(dest, src, 10);
    EXPECT_LT(ret, 0);
}

TEST(customTest, strlenCase)
{
    char src[50] = "this is source";

    size_t ret = customStrlen(NULL);
    EXPECT_EQ(ret, 0);
    ret = customStrlen(src);
    EXPECT_EQ(ret, 14);
}

TEST(customTest, strstrCase)
{
    char src[50] = "this is source";

    char *ret = customStrstr(src, NULL);
    EXPECT_STREQ(ret, NULL);
    ret = customStrstr(src, "");
    EXPECT_STREQ(ret, src);
    ret = customStrstr(src, "is");
    EXPECT_STREQ(ret, "is is source");
    ret = customStrstr(src, "src");
    EXPECT_STREQ(ret, NULL);
}

TEST(customTest, atoiCase)
{
    char str[5] = "1234";
    char str2[5] = "abcd";
    char str3[] = "2147483649";
    char str4[] = "-2147483649";

    int ret = customAtoi(str);
    EXPECT_EQ(ret, 1234);
    ret = customAtoi(str2);
    EXPECT_EQ(ret, 0);
    ret = customAtoi(str3);
    EXPECT_EQ(ret, 0);
    ret = customAtoi(str4);
    EXPECT_EQ(ret, 0);
}

TEST(customTest, memmetCase)
{
    char str[5] = "abcd";

    char *ret = (char *)customMemset(NULL, 0, 5);
    EXPECT_STRNE(str, NULL);
    EXPECT_STREQ(ret, NULL);
    ret = (char *)customMemset(str, 0, sizeof(str));
    EXPECT_STREQ(ret, "");
    EXPECT_STREQ(str, "");
}

int main(int argc, char **argv)
{
    printf("Running main() from %s\n\n", __FILE__);
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}