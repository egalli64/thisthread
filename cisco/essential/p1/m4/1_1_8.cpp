/*
    Cisco Network Academy - CPA - Programming Essentials in C++
    https://www.netacad.com/courses/programming/essentials-programming-c-plus-plus

    My repository: https://github.com/egalli64/thisthread

    1.1.8 Arrays of pointers as multidimensional arrays

    a triangular matrix
 */
#include <iostream>

int main()
{
    constexpr int n_rows{5};

    int **tri_matrix = new int *[n_rows];
    for (int i = 0; i < n_rows; ++i)
    {
        tri_matrix[i] = new int[i + 1];
        for (int j = 0; j <= i; j++)
        {
            tri_matrix[i][j] = i * 10 + j;
        }
    }

    for (int i = 0; i < n_rows; i++)
    {
        for (int j = 0; j <= i; j++)
        {
            std::cout << tri_matrix[i][j] << " ";
        }
        std::cout << '\n';
    }

    for (int i = 0; i < n_rows; i++)
    {
        delete[] tri_matrix[i];
    }
    delete[] tri_matrix;
}
