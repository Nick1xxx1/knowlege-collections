#include "gtest/gtest.h"
#include "sort.h"
#include <iostream>
#include <algorithm>
#include <time.h>

using namespace std;

static void randomVec(vector<int> &vecToRand)
{
    srand(time(NULL));
    int step = rand() % 3 + 1;
    int size = vecToRand.size();
    step %= size;
    reverse(vecToRand.begin(), vecToRand.end());
    reverse(vecToRand.begin(), vecToRand.begin() + step);
    reverse(vecToRand.begin() + step, vecToRand.end());
}

TEST(sortTest, bubbleSortCase)
{
    vector<int> emptyVec;
    int ret = bubbleSort(emptyVec);
    EXPECT_EQ(ret, -1);

    vector<int> testVec = {1, 2, 3, 4, 5, 6};
    vector<int> randVec = testVec;
    randomVec(randVec);
    EXPECT_NE(testVec, randVec);
    ret = bubbleSort(randVec);
    EXPECT_EQ(testVec, randVec);
    EXPECT_EQ(ret, 0);
}

TEST(sortTest, selectSortCase)
{
    vector<int> emptyVec;
    int ret = selectSort(emptyVec);
    EXPECT_EQ(ret, -1);

    vector<int> testVec = {1, 2, 3, 4, 5, 6};
    vector<int> randVec = testVec;
    randomVec(randVec);
    EXPECT_NE(testVec, randVec);
    ret = selectSort(randVec);
    EXPECT_EQ(testVec, randVec);
    EXPECT_EQ(ret, 0);
}

TEST(sortTest, insertSortCase)
{
    vector<int> emptyVec;
    int ret = insertSort(emptyVec);
    EXPECT_EQ(ret, -1);

    vector<int> testVec = {1, 2, 3, 4, 5, 6};
    vector<int> randVec = testVec;
    randomVec(randVec);
    EXPECT_NE(testVec, randVec);
    ret = insertSort(randVec);
    EXPECT_EQ(testVec, randVec);
    EXPECT_EQ(ret, 0);
}

TEST(sortTest, shellSortCase)
{
    vector<int> emptyVec;
    int ret = shellSort(emptyVec);
    EXPECT_EQ(ret, -1);

    vector<int> testVec = {1, 2, 3, 4, 5, 6};
    vector<int> randVec = testVec;
    randomVec(randVec);
    EXPECT_NE(testVec, randVec);
    ret = shellSort(randVec);
    EXPECT_EQ(testVec, randVec);
    EXPECT_EQ(ret, 0);
}

TEST(sortTest, quickSortCase)
{
    vector<int> testVec = {1, 2, 3, 4, 5, 6};
    vector<int> randVec = testVec;
    randomVec(randVec);
    EXPECT_NE(testVec, randVec);
    quickSort(randVec, 0, randVec.size() - 1);
    EXPECT_EQ(testVec, randVec);
}

TEST(sortTest, mergeSortCase)
{
    vector<int> testVec = {1, 2, 3, 4, 5, 6};
    vector<int> randVec = testVec;
    randomVec(randVec);
    EXPECT_NE(testVec, randVec);
    mergeSort(randVec, 0, randVec.size() - 1);
    EXPECT_EQ(testVec, randVec);
}

TEST(sortTest, heapSortCase)
{
    vector<int> testVec = {1, 2, 3, 4, 5, 6};
    vector<int> randVec = testVec;
    randomVec(randVec);
    EXPECT_NE(testVec, randVec);
    heapSort(randVec);
    EXPECT_EQ(testVec, randVec);
}

int main(int argc, char **argv)
{
    printf("Running main() from %s\n\n", __FILE__);
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
