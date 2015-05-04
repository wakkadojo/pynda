#ifndef INTERACTOR_HPP
#define INTERACTOR_HPP

#include <vector>
#include <iostream>
#include <cmath>
#include <stdexcept>
#include <boost/serialization/serialization.hpp>
#include "linalg.hpp"
#include "utils.hpp"
#include "body.hpp"

// contains sphere interactions
class interactor
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
        interactor () { mu = 0.2; cor = 0.9; }
        interactor (double mu, double cor) { this->mu=mu; this->cor=cor; }
        // sphere-sphere
        void interact (sphere &, sphere &);
        // sphere-brick
        void interact (sphere & s, brick & b) { interact (b, s); }
        void interact (brick &, sphere &);
};

#endif // INTERACTOR_HPP
