#include <iostream>
#include <cstdlib>
#include <string>

#include <ecl/static_map.hpp>

enum class E
{
      e1
    , e2
    , e3
    , e4
};

using key_type   = E;
using value_type = const char*;

struct point
{
    point(double x_, double y_, double z_)                              noexcept
        : x(x_)
        , y(y_)
        , z(z_)
    {}

    point()                                                             noexcept
        : point(0, 0, 0)
    {}

    double x {};
    double y {};
    double z {};
};

std::ostream& operator<<(std::ostream& os, const point& p)
{
    os << "( " << "x: " << p.x
       << "; " << "y: " << p.y
       << "; " << "z: " << p.z << ")";

    return os;
}

auto m = ecl::create_map<key_type, value_type>
(
      "not found"
    , std::make_pair(E::e1, "e1")
    , std::make_pair(E::e2, "e2")
    , std::make_pair(E::e3, "_e3")
    , std::make_pair(E::e3, "e3") // overwrite E::e3 element
);

auto m2 = ecl::create_map<key_type, point>
(
      { 0, 0, 0 }
    , std::make_pair(E::e1, point { 0, 0, 0 } )
    , std::make_pair(E::e2, point { 1, 1, 1 } )
    , std::make_pair(E::e3, point { 2, 2, 2 } )
    , std::make_pair(E::e3, point { 3, 3, 3 } ) // overwrite E::e3 element
);

auto m3 = ecl::create_map<const char*, const char*>
(
      "not found"
    , std::make_pair("k1", "v1")
    , std::make_pair("k2", "v2")
    , std::make_pair("k3", "v3")
    , std::make_pair("k4", "v4")
    , std::make_pair("k5", "v5")
);

int main(int, char**, char**)
{
    std::cout << "map elements count: " << m.size() << std::endl;

    std::cout << "range-for interation over map: ";
    for(auto& p : m)
    {
        std::cout << p << " ";
    }
    std::cout << std::endl;

    std::cout << "map[E::e1]: " << m[E::e1] << std::endl;
    std::cout << "map[E::e2]: " << m[E::e2] << std::endl;
    std::cout << "map[E::e3]: " << m[E::e3] << std::endl;
    std::cout << "map[E::e4]: " << m[E::e4] << std::endl;

    std::cout << "map elements count: " << m2.size() << std::endl;

    std::cout << "range-for interation over map: ";
    for(auto& p : m2)
    {
        std::cout << p << " ";
    }
    std::cout << std::endl;

    std::cout << "map[E::e1]: " << m2[E::e1] << std::endl;
    std::cout << "map[E::e2]: " << m2[E::e2] << std::endl;
    std::cout << "map[E::e3]: " << m2[E::e3] << std::endl;
    std::cout << "map[E::e4]: " << m2[E::e4] << std::endl;

    std::cout << "map elements count: " << m3.size() << std::endl;

    std::cout << "range-for interation over map: ";
    for(auto& p : m3)
    {
        std::cout << p << " ";
    }
    std::cout << std::endl;

    std::cout << "map[\"k1\"]: " << m3["k1"] << std::endl;
    std::cout << "map[\"k2\"]: " << m3["k2"] << std::endl;
    std::cout << "map[\"k3\"]: " << m3["k3"] << std::endl;
    std::cout << "map[\"k4\"]: " << m3["k4"] << std::endl;
    std::cout << "map[\"k5\"]: " << m3["k5"] << std::endl;
    std::cout << "map[\"k6\"]: " << m3["k6"] << std::endl;
}
