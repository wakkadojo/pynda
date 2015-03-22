#include "linalg.h"
#include "body.h"
#include "world.h"
#include "utils.h"
#include <iostream>
#include <cmath>

void list_test ()
{
    list<double> s;
    s.insert (1.0);
    s.insert (2.0);
    s.insert (3.0);
    s.insert (4.0);
    s.insert (5.0);
    double & val = s.get ();
    s.clear ();
    for (s.seek (); !s.at_end(); s.next ())
    {
        std::cout << s.get () << val << std::endl;
    }
}

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
