/**
 * info: http://thisthread.blogspot.com/2014/03/heapsort.html 
 */

#include <vector>
#include <algorithm>

typedef std::vector<int> Vector;

unsigned childLeft(unsigned i) { return (2 * i) + 1; }
unsigned childRight(unsigned i) { return (2 * i) + 2; }

void maxHeapify(Vector& data, unsigned i, unsigned len)
{
  unsigned left = childLeft(i);
  unsigned right = childRight(i);

  unsigned largest = (left < len && (data[left] > data[i])) ? left : i;
  if(right < len && (data[right] > data[largest]))
    largest = right;

  if(largest != i)
  {
    std::swap(data[i], data[largest]);
    maxHeapify(data, largest, len);
  }
}

void buildMaxHeap(Vector& data)
{
  for(int i = data.size() / 2; i >= 0; --i)
    maxHeapify(data, i, data.size());
}

void sortHeap(Vector& heap)
{
  for(int i = heap.size() - 1; i > 0; --i)
  {
    std::swap(heap[0], heap[i]);
    maxHeapify(heap, 0, i);
  }
}

void heapsort(Vector& data)
{
  buildMaxHeap(data); // std::make_heap(data.begin(), data.end());
  sortHeap(data); // std::sort_heap(data.begin(), data.end());
}

#include <gtest/gtest.h>

TEST(HS, simple)
{
  Vector data { 42, 12, 91 };
  heapsort(data);
  for(unsigned i = 1; i < data.size(); ++i)
    EXPECT_TRUE(data[i] > data[i-1]);
}

TEST(HS, simple2)
{
  Vector data { 4, 1, 3, 2, 16, 9, 10, 14, 8, 7 };
  heapsort(data);
  for(unsigned i = 1; i < data.size(); ++i)
    EXPECT_TRUE(data[i] > data[i-1]);
}
