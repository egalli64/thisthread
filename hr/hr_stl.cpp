/**
 * HackerRank C++ STL Challenges
 * https://www.hackerrank.com/domains/cpp/stl
 * Author:   Manny egalli64@gmail.com
 * Info:     http://thisthread.blogspot.com/
 */
#include<algorithm>
#include<deque>
#include<vector>

#include "gtest/gtest.h"

using VInt = std::vector<int>;
using DInt = std::deque<int>;

namespace
{
    /**
     * HackerRank C++ STL Deque-STL
     * https://www.hackerrank.com/challenges/deque-stl/problem
     * calling max_element() each time makes this algorithm too slow 
     */
    VInt maxInSubsPlain(int data[], int n, int k)
    {
        DInt buffer(data, data + k);
        VInt result{ *std::max_element(buffer.begin(), buffer.end()) };
        for (int i = k; i < n; ++i)
        {
            buffer.pop_front();
            buffer.push_back(data[i]);
            result.push_back(*std::max_element(buffer.begin(), buffer.end()));
        }

        return result;
    }

    /**
    * HackerRank C++ STL Deque-STL
    * https://www.hackerrank.com/challenges/deque-stl/problem
    * moving window approach, call max_element() only when in doubt
    */
    VInt maxInSubs(int data[], size_t n, size_t k)
    {
        VInt result{ *std::max_element(data, data + k) };

        for (size_t beg = 1, end = k + 1; end <= n; ++beg, ++end)
        {
            if (data[end - 1] > result[result.size() - 1])
            {
                result.push_back(data[end - 1]);
            }
            else if (data[beg - 1] < result[result.size() - 1])
            {
                result.push_back(result[result.size() - 1]);
            }
            else
            {
                result.push_back(*std::max_element(data + beg, data + end));
            }
        }

        return result;
    }
}

TEST(TestMaxInSubsPlain, Given1)
{
    int data[] = { 3, 4, 6, 3, 4 };
    auto result = maxInSubsPlain(data, 5, 2);
    ASSERT_EQ(4, result.size());
    EXPECT_EQ(4, result[0]);
    EXPECT_EQ(6, result[1]);
    EXPECT_EQ(6, result[2]);
    EXPECT_EQ(4, result[3]);
}

TEST(TestMaxInSubsPlain, Given2)
{
    int data[] = { 3, 4, 5, 8, 1, 4, 10 };
    auto result = maxInSubsPlain(data, 7, 4);
    ASSERT_EQ(4, result.size());
    EXPECT_EQ(8, result[0]);
    EXPECT_EQ(8, result[1]);
    EXPECT_EQ(8, result[2]);
    EXPECT_EQ(10, result[3]);
}


TEST(TestMaxInSubs, Given1)
{
    int data[] = { 3, 4, 6, 3, 4 };
    auto result = maxInSubs(data, 5, 2);
    ASSERT_EQ(4, result.size());
    EXPECT_EQ(4, result[0]);
    EXPECT_EQ(6, result[1]);
    EXPECT_EQ(6, result[2]);
    EXPECT_EQ(4, result[3]);
}

TEST(TestMaxInSubs, Given2)
{
    int data[] = { 3, 4, 5, 8, 1, 4, 10 };
    auto result = maxInSubs(data, 7, 4);
    ASSERT_EQ(4, result.size());
    EXPECT_EQ(8, result[0]);
    EXPECT_EQ(8, result[1]);
    EXPECT_EQ(8, result[2]);
    EXPECT_EQ(10, result[3]);
}


#ifdef STANDALONE

#include <iterator>
using namespace std;

void printKMax(int arr[], int n, int k)
{
    auto data = maxInSubs(arr, n, k);
    std::copy(data.begin(), data.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << '\n';
}

int main()
{
    int t;
    cin >> t;
    while (t > 0) {
        int n, k;
        cin >> n >> k;
        int i;
        int arr[n];
        for (i = 0; i < n; i++)
            cin >> arr[i];
        printKMax(arr, n, k);
        t--;
    }
    return 0;
}

#endif // STANDALONE

