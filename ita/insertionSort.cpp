#include <vector>

void insertionSort(std::vector<int>& data)
{
    for(unsigned i = 1; i < data.size(); ++i)
    {
        int value = data[i];
        int j = i - 1;
        while(j >= 0 && data[j] > value)
        {
            data[j+1] = data[j];
            --j;
        }
        data[j+1] = value;
    }
}

#include <gtest/gtest.h>

TEST(InsertionSort, Simple)
{
    std::vector<int> data { 6, 3, 0, 5, 2, 1, 4 };
    insertionSort(data);
    for(unsigned i = 0; i < data.size(); ++i)
        ASSERT_EQ(i, data[i]);
}
