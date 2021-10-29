#include "gtest/gtest.h"
extern "C"
{
#include "kmp.h"
}

#include <string.h>

TEST(testKmp, match)
{
    char text[50] = "asdasflnsdljvnsldjvnlasdjflewjr";
    char pattern[10] = "asdas";
    char pattern2[10] = "xcvsdf";
    char pattern3[10] = "jvnsldj";

    int next[50];
    int ret = kmp(text, pattern, next);
    EXPECT_EQ(ret, 0);
    ret = kmp(text, pattern2, next);
    EXPECT_EQ(ret, -1);
    ret = kmp(text, pattern3, next);
    EXPECT_EQ(ret, 11);
}

int main(int argc, char **argv)
{
    printf("Running main() from %s\n\n", __FILE__);
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}