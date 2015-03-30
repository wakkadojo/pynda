#ifndef BODY_H
#define BODY_H

#include <vector>
#include <iostream>
#include <cmath>
#include <boost/serialization/serialization.hpp>
#include "constants.hpp"
#include "linalg.hpp"

struct sphere
{
    // just needs access for saving
    friend class boost::serialization::access;
    template <class Archive> void serialize (Archive & ar, unsigned int version)
    {
        ar & m & r & I;
        ar & x & v & q & w;
        ar & flag;
    }
    double m;    // mass
    double r;    // radius
    double I;    // moment of inertia
    vec3d x;     // sphere position
    vec3d v;     // sphere velocity
    vec3d q;     // orientation
    vec3d w;     // rotational velocity
    int flag;    // is the sphere moving or static
    // TODO: stress tensor
    sphere ();
    sphere (double, double, vec3d, vec3d);
    sphere (double, double, double, vec3d, vec3d, vec3d);
    void init (double, double, double, vec3d, vec3d, vec3d);
};

// contains sphere interactions
// TODO: function pointers and custom interaction functions (sticking)
class body_interactor
{
    friend class boost::serialization::access;
    template <class Archive> void serialize (Archive & ar, unsigned int version)
    { 
        ar & mu & cor; 
    }
    double mu, cor; // friction and coefficient of restitution
    public:
        // TODO: include stuck grains (m -> infinity etc)
        body_interactor () { mu = 0; cor = 0.9; }
        body_interactor (double mu, double cor) { this->mu=mu; this->cor=cor; }
        void interact (sphere &, sphere &);
};

#endif // BODY_H
