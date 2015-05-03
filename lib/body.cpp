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

void body_interactor::one_moving_interact (sphere & si, sphere & sj)
{
    if (si.flag != sphere::state::moving or sj.flag != sphere::state::fixed)
        throw std::invalid_argument ("one_moving_interact needs first sphere moving and second sphere fixed");

    // points from 1 to 2
    vec3d dx = sj.x - si.x;

    if (dx*dx + constants::eps > (si.r + sj.r)*(si.r + sj.r))
        return;

    // The normal vector, used for both impulse and offset
    double dx_n = dx.norm ();
    vec3d n = dx/dx_n; 
    // SPHERE OFFSETS
    // offset about the center of mass
    // the amount they are overlapping is the amount we wish to offset them
    double s = si.r + sj.r - dx_n;

    vec3d dxi = -(sj.m*s/(si.m + sj.m))*n;
    vec3d dxj =  (si.m*s/(si.m + sj.m))*n;
    // END SPHERE OFFSETS

    // COLLISION IMPULSE
    // speed difference
    vec3d dv = si.v + si.r*si.w.cross (n) - (sj.v - sj.r*sj.w.cross (n));
    
    // normal impulse
    double fn = -(1.0+cor)*(dv*n)/(1.0/si.m + 1.0/sj.m);
    // tangential unit vector
    vec3d t = dv - (dv*n)*n; 
    double t_n = t.norm ();
    t = t_n > constants::eps ? t/t_n : t;
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
void body_interactor::two_moving_interact (sphere & si, sphere & sj)
{
    // points from 1 to 2
    vec3d dx = sj.x - si.x;

    if (dx*dx + constants::eps > (si.r + sj.r)*(si.r + sj.r))
        return;

    // The normal vector, used for both impulse and offset
    double dx_n = dx.norm ();
    vec3d n = dx/dx_n; 
    // SPHERE OFFSETS
    // offset about the center of mass
    // the amount they are overlapping is the amount we wish to offset them
    double s = si.r + sj.r - dx_n;

    vec3d dxi = -(sj.m*s/(si.m + sj.m))*n;
    vec3d dxj =  (si.m*s/(si.m + sj.m))*n;
    // END SPHERE OFFSETS

    // COLLISION IMPULSE
    // speed difference
    vec3d dv = si.v + si.r*si.w.cross (n) - (sj.v - sj.r*sj.w.cross (n));
    
    // normal impulse
    double fn = -(1.0+cor)*(dv*n)/(1.0/si.m + 1.0/sj.m);
    // tangential unit vector
    vec3d t = dv - (dv*n)*n; 
    double t_n = t.norm ();
    t = t_n > constants::eps ? t/t_n : t;
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

void body_interactor::interact (sphere & si, sphere & sj)
{
    two_moving_interact (si, sj);
}

void body_interactor::interact (brick & b, sphere & s)
{
    // edges and corners can be computationally intensive, maybe don't check?
    unsigned int d = constants::d;

    // Main faces
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
    // edges -- doo all aligned with x, y, z axes in each main loop
    // each edge is the segment between two corners, and each loop uses all 8 corners
    for (unsigned int i=0; i<d; ++i)
    {
        vec3d t, u, v; 
        t[i] = u[(i+1)%d] = v[(i+2)%d] = 1;
        // always start with respect to bottom-left edge
        // edges are between c_i and c_i+1
        std::vector<vec3d> c = {vec3d (), vec3d (), vec3d (), vec3d (),
                                vec3d (), vec3d (), vec3d (), vec3d ()};
        c[0] = b.x - b.L/2.0;         c[1] = c[0] + b.L[i]*t;
        c[2] = c[0] + b.L[(i+1)%d]*u; c[3] = c[2] + b.L[i]*t;
        c[4] = c[0] + b.L[(i+2)%d]*v; c[5] = c[4] + b.L[i]*t;
        c[6] = c[2] + b.L[(i+2)%d]*v; c[7] = c[6] + b.L[i]*t;
        for (unsigned int j=0; j<c.size ()/2; ++j)
            // whether or not in between two points in t axis
            if (s.x[i] > c[2*j][i] and s.x[i] < c[2*j+1][i])
            {
                // distance from the sphere to one of the line points
                vec3d a = c[2*j] - s.x;
                vec3d p = a - (a*t)*t; // shortest vector pointing from sphere to line
                double distance = p.norm (); // distance from edge
                if (distance < s.r) // collision
                {
                    vec3d n = p/distance; // normal vector pointing toward sphere
                    // realign so touching edge
                    s.x = s.x - (s.r - distance)*n;
                    // reverse speed
                    s.v = s.v*n > 0 ? s.v - 2*(s.v*n)*n : s.v; 
                }
            }

    }
    // corners
}

// copy constructor
body_interactor& body_interactor::operator= (const body_interactor &other)
{
            mu = other.mu;
            cor = other.cor;
            return *this;
}

// End Body Interactor
// 
