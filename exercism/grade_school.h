/*
    Exercism C++ track

    Source: https://exercism.org/tracks/cpp
    My solutions: https://github.com/egalli64/thisthread exercism

    Grade School https://exercism.org/tracks/cpp/exercises/grade-school/
 */
#pragma once

#include <algorithm>
#include <map>
#include <string>
#include <vector>

namespace grade_school
{
    class school
    {
        std::map<int, std::vector<std::string>> roster_;

    public:
        const std::map<int, std::vector<std::string>> roster() const
        {
            return roster_;
        }

        void add(const std::string &name, int x)
        {
            auto grade = &roster_[x];
            grade->push_back(name);
            std::sort(grade->begin(), grade->end());
        }

        std::vector<std::string> grade(int x) const
        {
            auto it = roster_.find(x);
            if (it == roster_.end())
            {
                return {};
            }
            else
            {
                return it->second;
            }
        }
    };
}
