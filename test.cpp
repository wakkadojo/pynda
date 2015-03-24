#include "linalg.hpp"
#include "body.hpp"
#include "world.hpp"
#include <iostream>
#include <vector>

int main (void)
{
    int n_spheres = 5;
    std::vector<sphere> spheres;
    for (int i=0; i<n_spheres-1; ++i)
        spheres.push_back (sphere (0.1, 1, vec3d (0.1, 0.1, 0.1), vec3d ()));
    spheres.push_back (sphere (0.1, 1, vec3d (0.5, 0.5, 0.5), vec3d ()));
    world w (spheres);
    return 0;
}
