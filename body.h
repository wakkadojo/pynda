#ifndef BODY_H
#define BODY_H

#include <vector>
#include <iostream>
#include "constants.h"
#include "linalg.h"

struct sphere_package_data
{
    double m, r, I;
    double x, y, z;
    double vx, vy, vz;
    double wx, wy, wz;
    int flag;
};

struct sphere
{
    double m;    // mass
    double r;    // radius
    double I;    // moment of inertia
    vec3d x;     // sphere position
    vec3d v;     // sphere velocity
    vec3d w;     // rotational velocity
    int flag;    // is the sphere moving or static
    // TODO: stress tensor
    private:
        void init (double, double, double, vec3d, vec3d, vec3d);
    public:
        sphere ();
        sphere (double, double, vec3d, vec3d);
        sphere (double, double, double, vec3d, vec3d, vec3d);
        sphere (sphere_package_data);
        sphere_package_data package ();
};

struct body_interact_data
{
    unsigned int i, j; // labels of the two interacting spheres
    vec3d f;          // impulse
    vec3d n;          // the normal vector, needed for torques (n*r1, etc)
    vec3d dxi, dxj;   // COM offsets
};

// contains sphere interactions
// TODO: function pointers and custom interaction functions (sticking)
class body_interactor
{
    double mu, cor; // friction and coefficient of restitution
    public:
        // TODO: include stuck grains (m -> infinity etc)
        body_interactor (double, double);
        body_interact_data interact (unsigned int, unsigned int, std::vector<sphere> &);
};

#endif // BODY_H
