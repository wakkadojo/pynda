#ifndef WORLD_HPP
#define WORLD_HPP

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <string>
#include <fstream>
#include <vector>
#include <stdexcept>
#include "linalg.hpp"
#include "body.hpp"
#include "grid.hpp"
#include "interactor.hpp"

class world
{
    // should choose timestep according to dt <= R/(100*U), and grid L ~ 2R

    friend class boost::serialization::access;
    template <class Archive> void serialize (Archive & ar, unsigned int version)
    {
        ar & spheres;
        ar & bricks;
        ar & bi;
        ar & c & min_box & max_box;
        ar & t & dt;
    }
    std::vector<sphere> spheres; // list of spheres
    std::vector<brick> bricks;
    interactor bi; // needs forces
    double t, dt;
    std::vector<unsigned int> c = { 0, 0, 0 };
    std::vector<double> min_box = { -0.5, -0.5, -0.5 };
    std::vector<double> max_box = {  0.5,  0.5,  0.5 };
    // time stepper?
    // if a timestep and grid size is set oorrectly, then the grid should not need
    // to be updated more than once every 100 steps, since that should be the 
    // minimum time for spheres to leave cells.
    const unsigned int grid_update_steps = 50;
    unsigned int grid_step_counter;
    grid g;
    private:
        void update_flags (); // update sphere flags i.e. based on box boundaries
        void clean (); // remove spheres flagged for deletion
    public:
        world ();
        world (const vec3d, const interactor, const double);
        void save (std::string);
        void load (std::string);
        void add_sphere (sphere);
        void add_brick (brick);
        unsigned int num_spheres ();
        unsigned int num_bricks ();
        sphere get_sphere (unsigned int);
        brick get_brick (unsigned int);
        void step ();
        void step (unsigned int);
};

#endif // WORLD_HPP
