/**
 * Solution to Google Jam Osmos problem
 *
 * info: http://thisthread.blogspot.com/2013/10/assert-death-and-deduction-on-osmos.html
 */

#include <cassert>
#include <vector>
#include <algorithm>

unsigned osmos(unsigned myMote, std::vector<unsigned>& motes)
{
  assert(myMote);

  // starving
  if(myMote == 1)
    return motes.size();

  std::sort(motes.begin(), motes.end());

  // biggest fish
  if(myMote > motes.back())
    return 0;

  unsigned removing = motes.size();
  unsigned adding = 0;
  for(unsigned i = 0; i < motes.size(); ++i)
  {
    if(myMote <= motes[i])
    {
      removing = std::min<unsigned>(removing, adding + motes.size() - i);
      while(myMote <= motes[i])
      {
        myMote += myMote - 1;
        ++adding;
      }
    }
    myMote += motes[i];
  }

  return std::min(removing, adding);
}

#include "gtest/gtest.h"

TEST(TestOsmos, CaseSample1)
{
  const unsigned myMote = 2;
  std::vector<unsigned> motes { 2, 1 };

  ASSERT_EQ(0, osmos(myMote, motes));
}

TEST(TestOsmos, CaseSample2)
{
  const unsigned myMote = 2;
  std::vector<unsigned> motes { 2, 1, 1, 6 };

  ASSERT_EQ(1, osmos(myMote, motes));
}

TEST(TestOsmos, CaseSample3)
{
  const unsigned myMote = 10;
  std::vector<unsigned> motes { 25, 20, 9, 100 };

  ASSERT_EQ(2, osmos(myMote, motes));
}

TEST(TestOsmos, CaseSample4)
{
  const unsigned myMote = 1;
  std::vector<unsigned> motes { 1, 1, 1, 1 };

  ASSERT_EQ(4, osmos(myMote, motes));
}

TEST(TestOsmos, CaseBigger)
{
  const unsigned myMote = 42;
  std::vector<unsigned> motes { 1, 40, 12, 7 };

  ASSERT_EQ(0, osmos(myMote, motes));
}

TEST(TestOsmos, CaseMixed)
{
  const unsigned myMote = 2;
  std::vector<unsigned> motes { 8, 15, 30, 40, 60 };

  ASSERT_EQ(3, osmos(myMote, motes));
}

TEST(TestOsmos, CaseBadMote)
{
  const unsigned myMote = 0;
  std::vector<unsigned> motes { 8, 15, 30, 40, 60 };

  ASSERT_DEATH(osmos(myMote, motes),"Assertion .* failed.");
}

TEST(TestOsmos, CaseBadMote2)
{
  const unsigned myMote = 10;
  std::vector<unsigned> motes { 0, 8, 15, 30, 40, 60 };

  ASSERT_EQ(0, osmos(myMote, motes));
//  ASSERT_DEATH(osmos(myMote, motes),"Assertion .* failed.");
}

