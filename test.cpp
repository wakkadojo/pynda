#include <iostream>
#include <vector>
#include <fstream>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include "linalg.hpp"
#include "body.hpp"
#include "world.hpp"

int main (void)
{
    int n_spheres = 5;
    std::vector<sphere> spheres;
    for (int i=0; i<n_spheres-1; ++i)
        spheres.push_back (sphere (0.1, 1, vec3d (0.1, 0.1, 0.1), vec3d ()));
    spheres.push_back (sphere (0.1, 1, vec3d (0.5, 0.5, 0.5), vec3d ()));
    
    world w (spheres);
    std::ofstream os ("boost_test.bin");
    boost::archive::binary_oarchive oa (os);
    oa << w;
    os.close ();
    
    std::vector<sphere> spheres2;
    world w2 (spheres2);
    std::cout << w2.count_spheres () << std::endl;
    std::ifstream is ("boost_test.bin");
    boost::archive::binary_iarchive ia (is);
    ia >> w2;
    is.close ();

    std::cout << w2.count_spheres () << std::endl;

    return 0;
}
