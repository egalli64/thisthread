/**
 * HackerRank C++ STL Challenges
 * https://www.hackerrank.com/domains/cpp/stl
 *
 * C++ STL Deque-STL
 * https://www.hackerrank.com/challenges/deque-stl/problem
 *
 * Author:   Manny egalli64@gmail.com
 * Info:     http://thisthread.blogspot.com/2018/05/hackerrank-deque-stl.html
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
     * naive solution
     *
     * calling max_element() each time makes this algorithm too slow
     */
    VInt maxInSubsPlain(int data[], int n, int k)
    {
        VInt results;
        for (int i = 0; i < n - k + 1; ++i)
        {
            results.push_back(*std::max_element(data + i, data + i + k));
        }

        return results;
    }

    /**
     * HackerRank C++ STL Deque-STL
     * 
     * call max_element() only when in doubt
     */
    VInt maxInSubsSimple(int data[], size_t n, size_t k)
    {
        VInt results{ *std::max_element(data, data + k) };

        for (size_t beg = 1, end = k + 1; end <= n; ++beg, ++end)
        {
            if (data[end - 1] > results[results.size() - 1])
            {
                results.push_back(data[end - 1]);
            }
            else if (data[beg - 1] < results[results.size() - 1])
            {
                results.push_back(results[results.size() - 1]);
            }
            else
            {
                results.push_back(*std::max_element(data + beg, data + end));
            }
        }

        return results;
    }

    /**
     * HackerRank C++ STL Deque-STL
     * 
     * linear time solution
     */

    /**
     * Push a new value as candidate,
     * but first remove values non-bigger than new entry
     */
    void pushBack(DInt& candidates, int data[], int i)
    {
        while (!candidates.empty() && data[i] >= data[candidates.back()])
            candidates.pop_back();
        candidates.push_back(i);
    }

    VInt maxInSubs(int data[], int n, int k)
    {
        VInt results;
        DInt candidates{ 0 };
        for (int i = 1; i < k; ++i)
        {
            pushBack(candidates, data, i);
        }

        for (int i = k; i < n; ++i)
        {
            results.push_back(data[candidates.front()]);

            if (candidates.front() <= i - k)
                candidates.pop_front();

            pushBack(candidates, data, i);
        }
        results.push_back(data[candidates.front()]);

        return results;
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


TEST(TestMaxInSubsSimple, Given1)
{
    int data[] = { 3, 4, 6, 3, 4 };
    auto result = maxInSubsSimple(data, 5, 2);
    ASSERT_EQ(4, result.size());
    EXPECT_EQ(4, result[0]);
    EXPECT_EQ(6, result[1]);
    EXPECT_EQ(6, result[2]);
    EXPECT_EQ(4, result[3]);
}

TEST(TestMaxInSubsSimple, Given2)
{
    int data[] = { 3, 4, 5, 8, 1, 4, 10 };
    auto result = maxInSubsSimple(data, 7, 4);
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
}

#endif // STANDALONE

