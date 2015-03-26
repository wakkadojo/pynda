#include <iostream>
#include <vector>
#include <fstream>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include "linalg.hpp"
#include "body.hpp"
#include "world.hpp"
#include "io.hpp"

int main (void)
{
    world w;

    int n_spheres = 5;
    for (int i=0; i<n_spheres-1; ++i)
        w.add_sphere (sphere (0.1, 1, vec3d (0.1, 0.1, 0.1), vec3d ()));
    w.add_sphere (sphere (0.1, 1, vec3d (0.5, 0.5, 0.5), vec3d ()));
    
    io::save (w, "boost_test.bin");
    
    world w2 = io::load ("boost_test.bin");

    std::cout << w2.count_spheres () << std::endl;

    return 0;
}
