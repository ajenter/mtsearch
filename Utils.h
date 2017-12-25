#pragma once
#include <iosfwd>
#include <iterator>

template <class Cont>
std::ostream& printContainer(std::ostream& os, const Cont& c)
{
    os << "{";
    auto start = std::cbegin(c), end = std::cend(c);
    if (start != end) {
        os << *start;
        for (auto it = ++start; it != end; ++it) {
            os << ", " << *it;
        }
    }

    os << "}";
    return os;
}
