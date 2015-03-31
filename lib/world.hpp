#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/vector.hpp>
#include "linalg.hpp"
#include "constants.hpp"
#include "body.hpp"

// grid used to find particle neighbors
class grid
{
    unsigned int n_cells;
    // stores the occupying particle indexes in each cell
    std::vector<std::vector<unsigned int>> cells;
    // stores the neighbors of each particle
    std::vector<std::vector<unsigned int>> neighbors; 
    // which cells should be searched for each given center cell
    // TODO: IN CASE OF LARGE MEMORY OVERHEAD, DO NOT MAKE FOR EVERY CELL, OR DO ON
    // THE GO
    std::vector<std::vector<unsigned int>> search_cells; 
    // cell numbers of the spheres
    std::vector<unsigned int> sphere_cells; 
    // the length of each dimension, centered at 0
    std::vector<double> box;
    // number of cells in each dimension
    std::vector<unsigned int> c;
    private:
        // sets which cells to look in for overlaps, should be called in constructor
        void set_search_cells ();
        // set cell # for spheres and also mark which cells have spheres
        void set_sphere_cells (const std::vector<sphere> &);
    public:
        grid ();
        grid (std::vector<unsigned int>, std::vector<double>);
        ~grid ();
        // TODO: once simulation is up and running, make new local cell 
        // searching algorithm
        // get neighbors of a sphere
        std::vector<unsigned int> get_neighbors (unsigned int); 
        // make grid and neighbor list
        void make_grid (const std::vector<sphere> &);
};

// TODO: oct-tree variant of grid

// Handles tracking spheres and finding neighbors etc
class world
{
    friend class boost::serialization::access;
    template <class Archive> void serialize (Archive & ar, unsigned int version)
    {
        ar & spheres;
        ar & bi;
        ar & t & dt;
    }
    std::vector<sphere> spheres; // list of spheres
    body_interactor bi; // needs forces
    double t, dt;
    std::vector<unsigned int> c = { 6, 6, 6 };
    std::vector<double> box = { 1.0, 1.0, 1.0 };
    // time stepper?
    grid g;
    public:
        // TODO: timestep, gather+apply impulses, save/load state
        world ();
        void add_sphere (sphere s) { spheres.push_back (s); }
        unsigned int num_spheres () { return spheres.size (); }
        sphere get_sphere (unsigned int i) 
        {
            if (i < spheres.size ())
                return spheres[i]; 
            // throw
        }
        void step ();
};

#endif // WORLD_H
