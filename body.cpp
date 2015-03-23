#include "body.h"
#include <iostream>
#include <cmath>

// Sphere
//

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

sphere::sphere (sphere_package_data spd)
{
    init (spd.r, spd.m, spd.I, vec3d (spd.x, spd.y, spd.z), 
          vec3d (spd.vx, spd.vy, spd.vz), vec3d (spd.wx, spd.wy, spd.wz));
}

void sphere::init (double r, double m, double I, vec3d x, vec3d v, vec3d w)
{
    this->m = m;
    this->r = r;
    this->I = I;
    this->x = x;
    this->v = v;
    this->w = w;
    flag = 0;
}

sphere_package_data sphere::package ()
{
    sphere_package_data spd;
    spd.m = m; spd.r = r; spd.I = I;
    spd.x = x[0]; spd.y = x[1]; spd.z = x[2];
    spd.vx = v[0]; spd.vy = v[1]; spd.vz = v[2];
    spd.wx = w[0]; spd.wy = w[1]; spd.wz = w[2];
    return spd;
}

// End Sphere
//

// Body Interactor
//

body_interactor::body_interactor (double mu, double cor)
{
    this->mu = mu;
    this->cor = cor;
}

// Returns interaction response from bodies. The response will contain no 
// offsets and forces if the spheres are not overlapping/interacting, but 
// generally, this function should NOT be called if spheres are not interacting
body_interact_data body_interactor::interact (unsigned int i, unsigned int j, std::vector<sphere> spheres)
{
    body_interact_data bid;
    // are these valid spheres
    if (i >= spheres.size () || j >= spheres.size ())
        throw "ERROR: collision indexes invalid";

    sphere si = spheres[i];
    sphere sj = spheres[j];

    // The normal vector, used for both impulse and offset
    vec3d dx = sj.x - si.x;
    vec3d n = dx/dx.norm (); // points from 1 to 2

    // SPHERE OFFSETS
    // offset about the center of mass
    // the amount they are overlapping is the amount we wish to offset them
    double s = si.r + sj.r - dx.norm ();
    vec3d dxi = -(sj.m*s/(si.m + sj.m))*n;
    vec3d dxj =  (si.m*s/(si.m + sj.m))*n;
    // END SPHERE OFFSETS

    // COLLISION IMPULSE
    // speed difference
    vec3d dv = si.v + si.r*si.w.cross (n) - (sj.v - sj.r*sj.w.cross (n));
    // normal impulse
    double fn = -(1.0+cor)*(dv*n)/(1.0/si.m + 1.0/sj.m);
    // tangential unit vector
    vec3d t  = dv - (dv*n)*n; t = t/t.norm ();
    // This is the tangential impulse that would equalize the post-collision
    // tangential veclocities of the two colliding objects. This is the max
    // impulse that can be felt, and is throttled by Coulombic friction.
    double ft_match_velocity = -1.0*(dv*t)/(1.0/si.m + 1.0/sj.m + si.r*si.r/si.I + sj.r*sj.r/sj.I);
    double sign_ft_match_velocity = ft_match_velocity > 0.0 ? 1.0 : -1.0;
    // tangential impulse. track sign of the impulse here since we use fabs
    double ft = fabs (ft_match_velocity) < mu*fabs (fn) ? 
        ft_match_velocity : sign_ft_match_velocity*fabs (fn)*mu;
    vec3d f = fn*n + ft*t;
    // END COLLISION IMPULSE
    
    // Now store the values (only store things that would change particles if
    // s > 0, i.e., they overlap. See s above. Generally, this function should 
    // not be called if particles are not overlapping, but we're nice
    bid.i   = i;
    bid.j   = j;
    bid.n   = n;
    bid.dxi = s > 0 ? dxi : vec3d ();
    bid.dxj = s > 0 ? dxj : vec3d ();
    bid.f   = s > 0 ? f : vec3d ();

    if (s < 0)
        std::cerr << "WARNING: called body interaction for bodies that did not interact." << std::endl;

    return bid;
}

// End Body Interactor
// 
