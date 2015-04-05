#include "body.hpp"

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

// End Sphere
//

// Body Interactor
//

// Returns interaction response from bodies. The response will contain no 
// offsets and forces if the spheres are not overlapping/interacting, but 
// generally, this function should NOT be called if spheres are not interacting
void body_interactor::interact (sphere & si, sphere & sj)
{
    // The normal vector, used for both impulse and offset
    vec3d dx = sj.x - si.x;
    vec3d n = dx/dx.norm (); // points from 1 to 2

    // SPHERE OFFSETS
    // offset about the center of mass
    // the amount they are overlapping is the amount we wish to offset them
    double s = si.r + sj.r - dx.norm ();
    // NOTICE do not interact if not in contact
    if (s < 0)
        return;

    vec3d dxi = -(sj.m*s/(si.m + sj.m))*n;
    vec3d dxj =  (si.m*s/(si.m + sj.m))*n;
    // END SPHERE OFFSETS

    // COLLISION IMPULSE
    // speed difference
    vec3d dv = si.v + si.r*si.w.cross (n) - (sj.v - sj.r*sj.w.cross (n));
    // normal impulse
    double fn = -(1.0+cor)*(dv*n)/(1.0/si.m + 1.0/sj.m);
    // tangential unit vector
    vec3d t = dv - (dv*n)*n; t = t.norm () > eps ? t/t.norm () : t;
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
    
    // not be called if particles are not overlapping, but we'll be extra safe
    dxi = s > 0 ? dxi : vec3d ();
    dxj = s > 0 ? dxj : vec3d ();
    f   = s > 0 ? f : vec3d ();

    // UPDATE SPHERES
    // position
    si.x = si.x + dxi;
    sj.x = sj.x + dxj;
    // translational velocity
    si.v = si.v + f/si.m;
    sj.v = sj.v - f/sj.m;
    // angular velocity
    si.w = si.w + si.r*n.cross (f)/si.I; // could alt. use n.cross (t)*ft
    sj.w = sj.w + sj.r*n.cross (f)/sj.I; // double negative
    // END UPDATE SPHERES
}

void body_interactor::interact (brick & b, sphere & s)
{
    // first check for overlap/intersection
    unsigned int d = b.x.size ();

    // TODO: CORNERS AND EDGES NOT HANDLED
    for (unsigned int i=0; i<d; ++i)
    {
        bool within = true;
        for (unsigned int j=i+1; j<i+d; ++j)
            if (s.x[j%d] > b.x[j%d] + b.L[j%d]/2.0 or
                s.x[j%d] < b.x[j%d] - b.L[j%d]/2.0)
                within = false;
        if (within)
        {
            if (fabs (s.x[i] - (b.x[i] - b.L[i]/2.0)) < s.r)
            {
                s.x[i] = b.x[i] - b.L[i]/2.0 - s.r;
                s.v[i] = s.v[i] < 0 ? s.v[i] : -s.v[i];
            }
            else if (fabs (s.x[i] - (b.x[i] + b.L[i]/2.0)) < s.r)
            {
                s.x[i] = b.x[i] + b.L[i]/2.0 + s.r;
                s.v[i] = s.v[i] > 0 ? s.v[i] : -s.v[i];
            }
        }
    }
}

// End Body Interactor
// 
