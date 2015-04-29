#ifndef UTILS_HPP
#define UTILS_HPP

#include <vector>
#include <algorithm>

namespace constants 
{
    const static double eps = 1e-9;
    const static unsigned int d = 3;
}

namespace helpers
{
    template<typename T>
    inline void remove_element (std::vector<T> & v, const T & item)
    {
        v.erase (std::remove (v.begin (), v.end (), item), v.end ());
    }
}

#endif // UTILS_HPP
