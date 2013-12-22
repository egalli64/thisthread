/*
 * coFish.cpp
 * A couple of solutions to the Codility Fish problem.
 *
 * For more info see:
 * http://thisthread.blogspot.com/2013/12/fish-eat-fish_22.html
 * http://thisthread.blogspot.com/2013/12/fish-eat-fish-linear-solution.html
 */

#include <vector>
#include <stack>
#include <numeric>

int solution77(std::vector<int>& sizes, std::vector<int>& directions)
{
    std::vector<bool> alive(sizes.size(), true);

    for (unsigned i = 0; i < sizes.size(); ++i)
    {
        if (directions[i] == 0) // up
        {
            for (int j = i - 1; j >= 0; --j)
            {
                if (directions[j] == 1)
                    alive[sizes[i] < sizes[j] ? i : j] = false;
                if (sizes[i] < sizes[j])
                    break;
            }
        }
        else // down
        {
            for (unsigned j = i + 1; j < sizes.size(); ++j)
            {
                if (directions[j] == 0)
                    alive[sizes[i] < sizes[j] ? i : j] = false;
                if (sizes[i] < sizes[j])
                    break;
            }
        }
    }

    return std::accumulate(alive.begin(), alive.end(), 0);
}

int solution(std::vector<int>& sizes, std::vector<int>& directions)
{
    int count = 0;
    std::stack<int> goingDown;
    for (unsigned i = 0; i < sizes.size(); ++i)
    {
        // going down
        if (directions[i] == 1)
        {
            goingDown.push(sizes[i]);
            continue;
        }

        // going up
        if (goingDown.empty())
        {
            ++count;
            continue;
        }

        while (!goingDown.empty())
        {
            if (sizes[i] > goingDown.top())
                goingDown.pop(); // eat it
            else
                break; // eaten
        }
        if (goingDown.empty())
            ++count; // not eaten
    }
    return count + goingDown.size();
}

#include <gtest/gtest.h>

TEST(Fish, Given)
{
    int a[] = { 4, 3, 2, 1, 5 };
    std::vector<int> sizes(a, a + 5);

    int b[] = { 0, 1, 0, 0, 0 };
    std::vector<int> directions(b, b + 5);

    ASSERT_EQ(2, solution(sizes, directions));
}

TEST(Fish, SameDirection)
{
    int a[] = { 4, 3, 2, 1, 5 };
    std::vector<int> sizes(a, a + 5);

    std::vector<int> directions(5);

    ASSERT_EQ(5, solution(sizes, directions));
}

TEST(Fish, SameDirection2)
{
    int a[] = { 4, 3, 2, 1, 5 };
    std::vector<int> sizes(a, a + 5);

    std::vector<int> directions(5, 1);

    ASSERT_EQ(5, solution(sizes, directions));
}

TEST(Fish, Mixed)
{
    int a[] = { 4, 3, 2, 1, 5 };
    std::vector<int> sizes(a, a + 5);
    int b[] = { 0, 1, 0, 1, 0 };
    std::vector<int> directions(b, b + 5);

    ASSERT_EQ(2, solution(sizes, directions));
}

TEST(Fish, NoMeeting)
{
    int a[] = { 4, 3, 2, 1, 5 };
    std::vector<int> sizes(a, a + 5);
    int b[] = { 0, 0, 0, 1, 1 };
    std::vector<int> directions(b, b + 5);

    ASSERT_EQ(5, solution(sizes, directions));
}

TEST(Fish, OneAgainst)
{
    int a[] = { 5, 3, 2, 1, 4 };
    std::vector<int> sizes(a, a + 5);
    int b[] = { 1, 0, 0, 0, 0 };
    std::vector<int> directions(b, b + 5);

    ASSERT_EQ(1, solution(sizes, directions));
}

TEST(Fish, OneAgainst2)
{
    int a[] = { 1, 2, 3, 4, 5 };
    std::vector<int> sizes(a, a + 5);
    int b[] = { 1, 1, 1, 1, 0 };
    std::vector<int> directions(b, b + 5);

    ASSERT_EQ(1, solution(sizes, directions));
}

TEST(Fish, OneAgainst3)
{
    int a[] = { 5, 3, 2, 1, 4 };
    std::vector<int> sizes(a, a + 5);
    int b[] = { 1, 1, 1, 1, 0 };
    std::vector<int> directions(b, b + 5);

    ASSERT_EQ(1, solution(sizes, directions));
}

TEST(Fish, Pilot)
{
    int a[] = { 1, 5, 3, 4, 2 };
    std::vector<int> sizes(a, a + 5);
    int b[] = { 1, 1, 0, 0, 0 };
    std::vector<int> directions(b, b + 5);

    ASSERT_EQ(2, solution(sizes, directions));
}

TEST(Fish, Big)
{
    std::vector<int> sizes(100000);
    for (unsigned i = 0; i < sizes.size(); ++i)
        sizes[i] = i + 1;
    std::vector<int> directions(100000);

    ASSERT_EQ(100000, solution(sizes, directions));
}
