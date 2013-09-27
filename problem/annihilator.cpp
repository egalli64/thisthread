/**
 * annihilator.cpp
 *
 * Info: http://thisthread.blogspot.com/2013/09/bidimensional-array-annihilator.html
 */

#include <array>

template<size_t N, size_t M>
void annihilator(std::array<std::array<int, N>, M>& data)
{
  std::array<bool, N> cols {};
  std::array<bool, M> rows {};

  for(unsigned i = 0; i < M; ++i)
    for(unsigned j = 0; j < N; ++j)
      if(data[i][j] == 0)
        rows[i] = cols[j] = true;

  for(unsigned i = 0; i < M; ++i)
    for(unsigned j = 0; j < N; ++j)
      if(rows[i] || cols[j])
        data[i][j] = 0;
}

void cAnnihilator(int* data, const unsigned nRows, const unsigned nCols)
{
  bool rows[nRows];
  for(unsigned i = 0; i < nRows; ++i)
    rows[i] = false;
  bool cols[nCols];
  for(unsigned i = 0; i < nCols; ++i)
    cols[i] = false;

  for(unsigned i = 0; i < nRows; ++i)
    for(unsigned j = 0; j < nCols; ++j)
      if(*(data + (i * nCols) + j) == 0)
        rows[i] = cols[j] = true;

  for(unsigned i = 0; i < nRows; ++i)
    for(unsigned j = 0; j < nCols; ++j)
      if(rows[i] || cols[j])
        *(data + (i * nCols) + j) = 0;
}

#include "gtest/gtest.h"

TEST(Annihilator, CaseSimple)
{
  std::array<std::array<int, 3>, 5> data {{
    {{ 1, 2, 3 }},
    {{ 4, 5, 6 }},
    {{ 7, 8, 9 }},
    {{ 10, 11, 12 }},
    {{ 13, 14, 15 }}
  }};

  unsigned row = 3;
  unsigned col = 2;
  data[row][col] = 0;

  annihilator(data);

  for(unsigned i = 0; i < data.size(); ++i)
    ASSERT_EQ(data[i][col], 0);
  for(unsigned i = 0; i < data[3].size(); ++i)
    ASSERT_EQ(data[row][i], 0);
}

TEST(Annihilator, CaseNothing)
{
  std::array<std::array<int, 3>, 2> data {{
    {{ 1, 2, 3 }},
    {{ 4, 5, 6 }}
  }};

  annihilator(data);

  for(unsigned i = 0; i < data.size(); ++i)
    for(unsigned j = 0; j < data[i].size(); ++j)
      ASSERT_NE(data[i][j], 0);
}

TEST(cAnnihilator, CaseSimple)
{
  const unsigned nRows = 5;
  const unsigned nCols = 3;
  int data[nRows][nCols] = {
    { 1, 2, 3 },
    { 4, 5, 6 },
    { 7, 8, 9 },
    { 10, 11, 0 },
    { 13, 14, 15 }
  };

  cAnnihilator(&data[0][0], nRows, nCols);

  for(unsigned i = 0; i < nRows; ++i)
    ASSERT_EQ(data[i][2], 0);
  for(unsigned i = 0; i < nCols; ++i)
    ASSERT_EQ(data[3][i], 0);
}

TEST(cAnnihilator, CaseNothing)
{
  const unsigned nRows = 2;
  const unsigned nCols = 3;
  int data[2][3] = {
    { 1, 2, 3 },
    { 4, 5, 6 }
  };

  cAnnihilator(&data[0][0], nRows, nCols);

  for(unsigned i = 0; i < nRows; ++i)
    for(unsigned j = 0; j < nCols; ++j)
      ASSERT_NE(data[i][j], 0);
}

