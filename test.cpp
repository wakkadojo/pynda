#include "quaternion.h"
#include "linalg.h"
#include "body.h"
#include "world.h"
#include "utils.h"
#include <iostream>
#include <cmath>

//void quaternion_test ()
//{
//    quaternion q = quaternion (1, 2, 3, 4);
//    quaternion q2 = quaternion (0, 1, 0, 0);
//    vec3d v = vec3d (1, 2, 3);
//    v = v/v.norm ();
//    quaternion qv = quaternion (0.0, v);
//    double n = q.norm ();
//    q = quaternion (q.get_scalar ()/n, q.get_vector ()/n);
//    //std::cout << q.get_rotation_matrix () << std::endl;
//    //std::cout << v*q.get_rotation_matrix ()*v << std::endl;
//    std::cout << q*qv*q.conjugate () << std::endl;
//    std::cout << q2*q*qv*q.conjugate ()*q2.conjugate () << std::endl;
//    std::cout << q*q2*qv*q2.conjugate ()*q.conjugate () << std::endl;
//}
// OLD -- from original rigid code
//void offset_test ()
//{
//    double r = 1.0;
//    double m = M_PI*r*r*r*4.0/2.0;
//    double I = m*2.0*r*r/5.0;
//    int n_spheres = 2;
//    sphere * spheres1 = new sphere [n_spheres];
//    spheres1[0] = sphere (r, m, I, vec3d (-0.6, 0.4, 0.0));
//    spheres1[1] = sphere (r, m, I, vec3d ( 1.4, 0.4, 0.0));
//    sphere * spheres2 = new sphere [n_spheres];
//    spheres2[0] = sphere (r, m, I, vec3d ( 1.6, -0.4, 0.0));
//    spheres2[1] = sphere (r, m, I, vec3d ( 2.6, -0.4, 0.0));
//    body b1 = body (spheres1, n_spheres);
//    body b2 = body (spheres2, n_spheres);
//
//    world c;
//
//    //b1.print_positions ();
//    //b2.print_positions ();
//    c.collide (b1, b2);
//    //b1.print_positions ();
//    //b2.print_positions ();
//}
//
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
