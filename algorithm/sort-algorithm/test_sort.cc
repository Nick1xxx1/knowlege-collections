#include <algorithm>
#include <iostream>
#include <string>

#include "gtest/gtest.h"
#include "sort.h"

using namespace std;

template<typename T>
static void RandomVec(vector<T> &vec_to_rand)
{
    srand(time(NULL));
    nc_int32_t step = rand() % 3 + 1;
    nc_int32_t size = vec_to_rand.size();
    step %= size;
    reverse(vec_to_rand.begin(), vec_to_rand.end());
    reverse(vec_to_rand.begin(), vec_to_rand.begin() + step);
    reverse(vec_to_rand.begin() + step, vec_to_rand.end());
}

TEST(sortTest, bubbleSortCase)
{
    vector<nc_int32_t> empty_vec;
    Result ret = BubbleSort(empty_vec);
    EXPECT_EQ(ret, Result::kError);

    vector<nc_int32_t> test_vec = {1, 2, 3, 4, 5, 6};
    vector<nc_int32_t> rand_vec = test_vec;
    RandomVec(rand_vec);
    EXPECT_NE(test_vec, rand_vec);
    ret = BubbleSort(rand_vec);
    EXPECT_EQ(test_vec, rand_vec);
    EXPECT_EQ(ret, Result::kOk);

    vector<string> test_vec2 = {"a", "b", "c", "d", "e", "f"};
    vector<string> rand_vec2 = test_vec2;
    RandomVec(rand_vec2);
    EXPECT_NE(test_vec2, rand_vec2);
    ret = BubbleSort(rand_vec2);
    EXPECT_EQ(test_vec2, rand_vec2);
    EXPECT_EQ(ret, Result::kOk);
}

TEST(sortTest, SelectSortCase)
{
    vector<nc_int32_t> empty_vec;
    Result ret = SelectSort(empty_vec);
    EXPECT_EQ(ret, Result::kError);

    vector<nc_int32_t> test_vec = {1, 2, 3, 4, 5, 6};
    vector<nc_int32_t> rand_vec = test_vec;
    RandomVec(rand_vec);
    EXPECT_NE(test_vec, rand_vec);
    ret = SelectSort(rand_vec);
    EXPECT_EQ(test_vec, rand_vec);
    EXPECT_EQ(ret, Result::kOk);

    vector<string> test_vec2 = {"a", "b", "c", "d", "e", "f"};
    vector<string> rand_vec2 = test_vec2;
    RandomVec(rand_vec2);
    EXPECT_NE(test_vec2, rand_vec2);
    ret = SelectSort(rand_vec2);
    EXPECT_EQ(test_vec2, rand_vec2);
    EXPECT_EQ(ret, Result::kOk);
}

TEST(sortTest, InsertSortCase)
{
    vector<nc_int32_t> empty_vec;
    Result ret = InsertSort(empty_vec);
    EXPECT_EQ(ret, Result::kError);

    vector<nc_int32_t> test_vec = {1, 2, 3, 4, 5, 6};
    vector<nc_int32_t> rand_vec = test_vec;
    RandomVec(rand_vec);
    EXPECT_NE(test_vec, rand_vec);
    ret = InsertSort(rand_vec);
    EXPECT_EQ(test_vec, rand_vec);
    EXPECT_EQ(ret, Result::kOk);

    vector<string> test_vec2 = {"a", "b", "c", "d", "e", "f"};
    vector<string> rand_vec2 = test_vec2;
    RandomVec(rand_vec2);
    EXPECT_NE(test_vec2, rand_vec2);
    ret = InsertSort(rand_vec2);
    EXPECT_EQ(test_vec2, rand_vec2);
    EXPECT_EQ(ret, Result::kOk);
}

TEST(sortTest, ShellSortCase)
{
    vector<nc_int32_t> empty_vec;
    Result ret = ShellSort(empty_vec);
    EXPECT_EQ(ret, Result::kError);

    vector<nc_int32_t> test_vec = {1, 2, 3, 4, 5, 6};
    vector<nc_int32_t> rand_vec = test_vec;
    RandomVec(rand_vec);
    EXPECT_NE(test_vec, rand_vec);
    ret = ShellSort(rand_vec);
    EXPECT_EQ(test_vec, rand_vec);
    EXPECT_EQ(ret, Result::kOk);

    vector<string> test_vec2 = {"a", "b", "c", "d", "e", "f"};
    vector<string> rand_vec2 = test_vec2;
    RandomVec(rand_vec2);
    EXPECT_NE(test_vec2, rand_vec2);
    ret = ShellSort(rand_vec2);
    EXPECT_EQ(test_vec2, rand_vec2);
    EXPECT_EQ(ret, Result::kOk);
}

TEST(sortTest, QuickSortCase)
{
    vector<int> test_vec = {1, 2, 3, 4, 5, 6};
    vector<int> rand_vec = test_vec;
    RandomVec(rand_vec);
    EXPECT_NE(test_vec, rand_vec);
    QuickSort(rand_vec, 0, rand_vec.size() - 1);
    EXPECT_EQ(test_vec, rand_vec);

    vector<string> test_vec2 = {"a", "b", "c", "d", "e", "f"};
    vector<string> rand_vec2 = test_vec2;
    RandomVec(rand_vec2);
    EXPECT_NE(test_vec2, rand_vec2);
    QuickSort(rand_vec2, 0, rand_vec2.size() - 1);
    EXPECT_EQ(test_vec2, rand_vec2);
}

TEST(sortTest, MergeSortCase)
{
    vector<int> test_vec = {1, 2, 3, 4, 5, 6};
    vector<int> rand_vec = test_vec;
    RandomVec(rand_vec);
    EXPECT_NE(test_vec, rand_vec);
    MergeSort(rand_vec, 0, rand_vec.size() - 1);
    EXPECT_EQ(test_vec, rand_vec);

    vector<string> test_vec2 = {"a", "b", "c", "d", "e", "f"};
    vector<string> rand_vec2 = test_vec2;
    RandomVec(rand_vec2);
    EXPECT_NE(test_vec2, rand_vec2);
    MergeSort(rand_vec2, 0, rand_vec2.size() - 1);
    EXPECT_EQ(test_vec2, rand_vec2);
}

TEST(sortTest, HeapSortCase)
{
    vector<int> test_vec = {1, 2, 3, 4, 5, 6};
    vector<int> rand_vec = test_vec;
    RandomVec(rand_vec);
    EXPECT_NE(test_vec, rand_vec);
    HeapSort(rand_vec);
    EXPECT_EQ(test_vec, rand_vec);

    vector<string> test_vec2 = {"a", "b", "c", "d", "e", "f"};
    vector<string> rand_vec2 = test_vec2;
    RandomVec(rand_vec2);
    EXPECT_NE(test_vec2, rand_vec2);
    HeapSort(rand_vec2);
    EXPECT_EQ(test_vec2, rand_vec2);
}

int main(int argc, char **argv)
{
    cout << "Running main() from " << __FILE__ << endl << endl;
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
