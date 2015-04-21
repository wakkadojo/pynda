#ifndef IO_HPP
#define IO_HPP

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <string>
#include <fstream>
#include "world.hpp"

class io 
{
    public:
        static void save (const world &, std::string);
        static world load (std::string);
};

#endif // IO_HPP
