/**
 * activitySelection.cpp
 *
 * info: http://thisthread.blogspot.com/2014/03/greedy-algorithm-for-activity-selection.html
 */

#include <utility>
#include <vector>

typedef std::pair<int, int> Activity;
typedef std::vector<Activity> Activities;

Activities selectMax(Activities& input)
{
  std::sort(input.begin(), input.end(), [](Activity a, Activity b) { return a.second < b.second; });

  Activities output;
  output.push_back(input[0]);

  for(unsigned i = 0, j = 1; j < input.size(); ++j)
  {
    if(input[j].first >= input[i].second)
    {
      output.push_back(input[j]);
      i = j;
    }
  }

  return output;
}

#include <gtest/gtest.h>

TEST(ActSel, Simple)
{
  Activities input { {1, 2}, {5, 9}, {0, 6}, {8, 9}, {3, 4}, {5, 7} };

  Activities output = selectMax(input);
  ASSERT_EQ(4, output.size());
  for(unsigned i = 1; i < output.size(); ++i)
    ASSERT_LE(output[i-1].second, output[i].first);
}

TEST(ActSel, Simple2)
{
  Activities input { {1, 4}, {3, 5}, {0, 6}, {3, 9}, {5, 9}, {5, 7}, {6, 10}, {8, 11}, {8, 12}, {2, 14}, {12, 16} };

  Activities output = selectMax(input);
  ASSERT_EQ(4, output.size());
  for(unsigned i = 1; i < output.size(); ++i)
    ASSERT_LE(output[i-1].second, output[i].first);
}

