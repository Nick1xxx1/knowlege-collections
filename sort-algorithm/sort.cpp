#include "sort.h"

using namespace std;

int bubbleSort(vector<int> &data)
{
    if (data.empty())
    {
        return -1;
    }

    int len = data.size();
    for (int i = 0; i < len; ++i)
    {
        int flag = 0;
        for (int j = 0; j < len - 1 - i; ++j)
        {
            if (data[j] > data[j + 1])
            {
                int tmp = data[j];
                data[j] = data[j + 1];
                data[j + 1] = tmp;
                flag = 1;
            }
        }

        if (flag == 0)
        {
            break;
        }
    }

    return 0;
}

int selectSort(vector<int> &data)
{
    if (data.empty())
    {
        return -1;
    }

    int len = data.size();
    for (int i = 0; i < len - 1; ++i)
    {
        for (int j = i + 1; j < len; ++j)
        {
            if (data[i] > data[j])
            {
                int tmp = data[i];
                data[i] = data[j];
                data[j] = tmp;
            }
        }
    }
    return 0;
}

int insertSort(vector<int> &data)
{
    if (data.empty())
    {
        return -1;
    }

    int i = 0;
    int j = 0;
    int len = data.size();

    for (i = 1; i < len; ++i)
    {
        int tmp = data[i];
        for (j = i - 1; data[j] > tmp && j >= 0; --j)
        {
            data[j + 1] = data[j];
        }
        data[j + 1] = tmp;
    }

    return 0;
}

int shellSort(vector<int> &data)
{
    if (data.empty())
    {
        return -1;
    }

    int gap = 0;
    int i = 0, j = 0;
    int len = data.size();

    for (gap = len / 2; gap >= 1; gap /= 2)
    {
        for (i = gap; i < len; ++i)
        {
            int tmp = data[i];
            for (j = i - gap; j >= 0 && tmp < data[j]; j -= gap)
            {
                data[j + gap] = data[j];
            }
            data[j + gap] = tmp;
        }
    }

    return 0;
}

static int partitionArray(vector<int> &data, int start, int end)
{
    int left = start;
    int right = end;
    int pivot = data[start];
    int index = start; // 哨兵的初始位置

    while (left < right)
    {
        while (left < right)
        {
            // 如果右边的值比哨兵的值大，则继续从右找
            if (pivot < data[right])
            {
                --right;
                continue;
            }
            // 找到比哨兵更小的值，则进行值替换，并记录此时哨兵的位置
            // 然后跳出循环
            data[index] = data[right];
            index = right;
            ++left;
            break;
        }

        while (left < right)
        {
            if (pivot >= data[left])
            {
                ++left;
                continue;
            }

            data[index] = data[left];
            index = left;
            --right;
            break;
        }
    }

    // 将哨兵的值重新赋值回去
    data[index] = pivot;
    // 返回哨兵的位置
    return index;
}

void quickSort(vector<int> &data, int start, int end)
{
    if (start >= end)
    {
        return;
    }

    int pivotIndex = partitionArray(data, start, end);
    quickSort(data, start, pivotIndex - 1);
    quickSort(data, pivotIndex + 1, end);
}

void mergeArray(vector<int> &data, int start, int mid, int end)
{
    vector<int> tmpVec(end - start + 1, 0);
    int pos1 = start;
    int pos2 = mid + 1;
    int pos = 0;

    while (pos1 <= mid && pos2 <= end)
    {
        if (data[pos1] <= data[pos2])
        {
            tmpVec[pos++] = data[pos1++];
        }
        else
        {
            tmpVec[pos++] = data[pos2++];
        }
    }

    while (pos1 <= mid)
    {
        tmpVec[pos++] = data[pos1++];
    }

    while (pos2 <= end)
    {
        tmpVec[pos++] = data[pos2++];
    }

    for (int i = 0; i < end - start + 1; ++i)
    {
        data[start + i] = tmpVec[i];
    }
}

void mergeSort(vector<int> &data, int start, int end)
{
    if (start >= end)
    {
        return;
    }

    int mid = start + (end - start) / 2;
    mergeSort(data, start, mid);
    mergeSort(data, mid + 1, end);
    mergeArray(data, start, mid, end);
}

void downAdjust(vector<int> &data, int parentIndex, int len)
{
    int parentValue = data[parentIndex];
    int childIndex = parentIndex * 2 + 1;
    while (childIndex < len)
    {
        if (childIndex + 1 < len && data[childIndex + 1] > data[childIndex])
        {
            childIndex++;
        }

        if (parentValue >= data[childIndex])
        {
            break;
        }

        data[parentIndex] = data[childIndex];
        parentIndex = childIndex;
        childIndex = parentIndex * 2 + 1;
    }

    data[parentIndex] = parentValue;
}

void heapSort(vector<int> &data)
{
    int len = data.size();
    for (int i = len / 2 - 1; i >= 0; --i)
    {
        downAdjust(data, i, len);
    }

    for (int i = len - 1; i > 0; --i)
    {
        int tmp = data[i];
        data[i] = data[0];
        data[0] = tmp;

        downAdjust(data, 0, i);
    }
}
