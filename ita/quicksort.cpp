/**
 * info: http://thisthread.blogspot.com/2014/03/quicksort.html
 */

#include <vector>
#include <algorithm>

typedef std::vector<int> Vector;

int partition(Vector& data, int left, int right)
{
  int pivot = data[right];
  int index = left - 1;

  for(int i = left; i < right; ++i)
  {
    if(data[i] <= pivot)
      std::swap(data[++index], data[i]);
  }

  std::swap(data[++index], data[right]);
  return index;
}

void quicksort(Vector& data, int left, int right)
{
  if(left < right)
  {
    int pivot = partition(data, left, right);
    quicksort(data, left, pivot - 1);
    quicksort(data, pivot + 1, right);
  }
}

#include <gtest/gtest.h>

TEST(QS, Simple)
{
  Vector data { 2, 8, 7, 1, 3, 2, 6, 4 };
  quicksort(data, 0, data.size() - 1);
  for(unsigned i = 1; i < data.size(); ++i)
    ASSERT_GE(data[i], data[i-1]);
}
