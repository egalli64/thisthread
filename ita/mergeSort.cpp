/**
 * info http://thisthread.blogspot.com/2014/02/merge-sort.html
 */

#include <vector>
#include <queue>

typedef std::vector<int> Data;
typedef std::queue<int> Queue;

void merge(Data& data, int left, int center, int right)
{
    Queue low;
    Queue high;

    for(int i = left; i <= center; ++i)
        low.push(data[i]);
    for(int i = center + 1; i <= right; ++i)
        high.push(data[i]);

    int i = left;
    while(!low.empty() && !high.empty())
    {
        if(low.front() <= high.front())
        {
            data[i++] = low.front();
            low.pop();
        }
        else
        {
            data[i++] = high.front();
            high.pop();
        }
    }

    while(!low.empty())
    {
        data[i++] = low.front();
        low.pop();
    }
    while(!high.empty())
    {
        data[i++] = high.front();
        high.pop();
    }
}

void mergeSort(Data& data, int left, int right)
{
    if(left < right)
    {
        int center = (left + right) / 2;
        mergeSort(data, left, center);
        mergeSort(data, center + 1, right);
        merge(data, left, center, right);
    }
}

#include "gtest/gtest.h"

TEST(MergeSort, Simple)
{
    std::vector<int> data { 6, 3, 0, 5, 2, 1, 4 };
    mergeSort(data, 0, data.size() - 1);
    for(unsigned i = 0; i < data.size(); ++i)
        ASSERT_EQ(i, data[i]);
}
