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

    nc_int32_t ret = Kmp(text, pattern);
    EXPECT_EQ(ret, 0);
    ret = Kmp(text, pattern2);
    EXPECT_EQ(ret, -1);
    ret = Kmp(text, pattern3);
    EXPECT_EQ(ret, 11);
    ret = Kmp(text, pattern4);
    EXPECT_EQ(ret, -1);
    ret = Kmp(text2, pattern);
    EXPECT_EQ(ret, -1);
    ret = Kmp(text2, pattern4);
    EXPECT_EQ(ret, -1);
    ret = Kmp(text3, pattern5);
    EXPECT_EQ(ret, 2);
}

int main(int argc, char **argv)
{
    printf("Running main() from %s\n\n", __FILE__);
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
