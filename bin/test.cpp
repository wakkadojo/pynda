#include <cstdlib>
#include <iostream>
#include <world.hpp>
#include <linalg.hpp>
#include <body.hpp>

double drand ()
{
    return (double) std::rand () / RAND_MAX;
}

int main (void)
{
    double R = 0.05;
    double r_av = R/25;
    double dt = r_av/100; // dt = 0.01 * R / U0
    vec3d cell_size (3.0*r_av, 3.0*r_av, 1.0);
    body_interactor bi (0.2, 0.9);
    world w (cell_size, bi, dt);
    unsigned int num_steps = 1000;
    unsigned int N_b = 0;
    unsigned int N_s = 0;

    for (unsigned int i=0; i<30; ++i)
    {
        w.step (num_steps);
        // injection rate = dN/dt = 2 R U_0 phi / Vs
        N_b += num_steps*dt*2*R*0.815/(3.1415*r_av*r_av);
        N_s += num_steps*dt*2*0.5*R*0.815/(3.1415*r_av*r_av);
        while (N_b-- > 0)
        {
            double r = (0.8 + 0.4*drand ())*r_av;
            double m = r*r;
            double I = m*r*r/2.0;
            vec3d v (1.0, 0.0, 0.0);
            vec3d x (R*(2*drand () + 0.5), 0.5 + R*(2*drand () - 1), 0.5);
            w.add_sphere (sphere (r, m, I, x, v, vec3d ()));
        }
        while (N_s-- > 0)
        {
            double r = (0.8 + 0.4*drand ())*r_av;
            double m = r*r;
            double I = m*r*r/2.0;
            vec3d v (1.0, 0.0, 0.0);
            vec3d x (1 - 0.5*R*(2*drand () + 0.5), 0.5 + 0.5*R*(2*drand () - 1), 0.5);
            w.add_sphere (sphere (r, m, I, x, v, vec3d ()));
        }
        std::cout << "done with " << i << std::endl;
    }
    return 0;
}
