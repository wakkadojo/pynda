#ifndef BODY_HPP
#define BODY_HPP

#include <vector>
#include <iostream>
#include <cmath>
#include <stdexcept>
#include <boost/serialization/serialization.hpp>
#include "linalg.hpp"
#include "utils.hpp"

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
    enum class state { moving, fixed, kill };
    double m;          // mass
    double r;          // radius
    double I;          // moment of inertia
    vec3d x;           // sphere position
    vec3d v;           // sphere velocity
    vec3d q;           // orientation
    vec3d w;           // rotational velocity
    state flag; // how should the sphere be treated
    // TODO: stress tensor
    sphere ();
    sphere (double, double, vec3d, vec3d);
    sphere (double, double, double, vec3d, vec3d, vec3d);
    void init (double, double, double, vec3d, vec3d, vec3d);
};

struct brick
{
    friend class boost::serialization::access;
    template <class Archive> void serialize (Archive & ar, unsigned int version)
    {
        ar & x & L;
    }
    vec3d x; // center
    vec3d L; // side lengths
    brick () { x = vec3d (); L = vec3d (); }
    brick (vec3d x, vec3d L) { this->x = x; this->L = L; }
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
    private:
        void one_moving_interact (sphere &, sphere &);
        void two_moving_interact (sphere &, sphere &);
    public:
        // TODO: include stuck grains (m -> infinity etc)
        body_interactor () { mu = 0.5; cor = 0.7; }
        body_interactor (double mu, double cor) { this->mu=mu; this->cor=cor; }
        // sphere-sphere
        void interact (sphere &, sphere &);
        // sphere-brick
        void interact (sphere & s, brick & b) { interact (b, s); }
        void interact (brick &, sphere &);
        // aux
        body_interactor& operator= (const body_interactor &);
};

#endif // BODY_HPP
