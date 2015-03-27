#include <iostream>
#include <sstream>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include "linalg.hpp"
#include "body.hpp"
#include "world.hpp"
#include "io.hpp"

int main (void)
{
    world w;

    w.add_sphere (sphere (0.1, 1, vec3d (0.1, 0.2, 0.0), vec3d (0.1, 0.5, 0.0)));
    w.add_sphere (sphere (0.1, 1, vec3d (0.5, 0.5, 0.0), vec3d (-0.2, 0.3, 0.0)));
    w.add_sphere (sphere (0.1, 1, vec3d (0.1, 0.5, 0.0), vec3d (0.2, -0.3, 0.0)));
    
    for (unsigned int i=0; i<10000; ++i)
    {
        w.step ();
        if (i % 100 == 0)
        {
            std::stringstream filename;
            filename << "states/state" << i/100 << ".bin";
            io::save (w, filename.str ());
        }
    }
    
    return 0;
}
