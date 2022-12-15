#include "gtest/gtest.h"

#include <string>

#include "kmp.h"

using std::string;

TEST(testKmp, match)
{
    string text("asdasflnsdljvnsldjvnlasdjflewjr");
    string text2("");
    string text3("ab cd");
    string pattern("asdas");
    string pattern2("xcvsdf");
    string pattern3("jvnsldj");
    string pattern4("");
    string pattern5(" ");

    nc_int32_t ret = kmp(text, pattern);
    EXPECT_EQ(ret, 0);
    ret = kmp(text, pattern2);
    EXPECT_EQ(ret, -1);
    ret = kmp(text, pattern3);
    EXPECT_EQ(ret, 11);
    ret = kmp(text, pattern4);
    EXPECT_EQ(ret, -1);
    ret = kmp(text2, pattern);
    EXPECT_EQ(ret, -1);
    ret = kmp(text2, pattern4);
    EXPECT_EQ(ret, -1);
    ret = kmp(text3, pattern5);
    EXPECT_EQ(ret, 2);
}

int main(int argc, char **argv)
{
    printf("Running main() from %s\n\n", __FILE__);
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
