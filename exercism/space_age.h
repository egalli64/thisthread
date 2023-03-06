/*
    Exercism C++ track

    Source: https://exercism.org/tracks/cpp
    My solutions: https://github.com/egalli64/thisthread exercism

    Space Age https://exercism.org/tracks/cpp/exercises/space-age/
 */
#pragma once

namespace space_age
{
    constexpr double earth_years_per_second = 1.0 / 31'557'600;
    constexpr double mercury_years_per_second = earth_years_per_second / 0.2408467;
    constexpr double venus_years_per_second = earth_years_per_second / 0.61519726;
    constexpr double mars_years_per_second = earth_years_per_second / 1.8808158;
    constexpr double jupiter_years_per_second = earth_years_per_second / 11.862615;
    constexpr double saturn_years_per_second = earth_years_per_second / 29.447498;
    constexpr double uranus_years_per_second = earth_years_per_second / 84.016846;
    constexpr double neptune_years_per_second = earth_years_per_second / 164.79132;

    class space_age
    {
    private:
        long long sec_;

    public:
        explicit space_age(long long sec) : sec_(sec) {}

        long long seconds() const { return sec_; }
        double on_earth() const { return sec_ * earth_years_per_second; }
        double on_mercury() const { return sec_ * mercury_years_per_second; };
        double on_venus() const { return sec_ * venus_years_per_second; };
        double on_mars() const { return sec_ * mars_years_per_second; };
        double on_jupiter() const { return sec_ * jupiter_years_per_second; };
        double on_saturn() const { return sec_ * saturn_years_per_second; };
        double on_uranus() const { return sec_ * uranus_years_per_second; };
        double on_neptune() const { return sec_ * neptune_years_per_second; };
    };
}