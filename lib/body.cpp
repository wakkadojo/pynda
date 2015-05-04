#include "body.hpp"

sphere::sphere ()
{
    init (1.0, 4.0*M_PI/3.0, 4.0*M_PI/3.0*2.0/5.0, vec3d (), vec3d (), vec3d ());
}

sphere::sphere (double r, double m, vec3d x, vec3d v)
{
    init (r, m, m*r*r*2.0/5.0, x, v, vec3d ());
}

sphere::sphere (double r, double m, double I, vec3d x, vec3d v, vec3d w)
{
    init (r, m, I, x, v, w);
}

void sphere::init (double r, double m, double I, vec3d x, vec3d v, vec3d w)
{
    this->m = m;
    this->r = r;
    this->I = I;
    this->x = x;
    this->v = v;
    this->w = w;
    flag = state::moving;
}
