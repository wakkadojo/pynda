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
    std::vector<std::vector<unsigned int>> cells;   // stores the occupying particle indexes in each cell
    std::vector<std::vector<unsigned int>> neighbors; // stores the neighbors of each particle
    std::vector<std::vector<unsigned int>> search_cells; // cells to search
    std::vector<unsigned int> sphere_cells; // cell numbers of the spheres
    std::vector<double> box;         // the length of each dimension, centered at 0
    std::vector<unsigned int> c;     // number of cells in each dimension
    private:
        void clear_cells ();     // empties each cell
        void clear_neighbors (); // empties each neighbor list
        void set_search_cells (); // sets which cells to look in for overlaps
        void set_sphere_cells (std::vector<sphere>); // set cell # for spheres
        void make_grid (std::vector<sphere>); // make grid and neighbor list
    public:
        grid ();
        grid (std::vector<unsigned int> &, std::vector<double> &, const std::vector<sphere> &);
        ~grid ();
        // TODO: once simulation is up and running, make new local cell 
        // searching algorithm
        std::vector<unsigned int> & get_neighbors (unsigned int); // get neighbors of a sphere
        std::vector<unsigned int> & get_spheres_in_cell (unsigned int); // return spheres in cell
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
    }
    std::vector<sphere> spheres; // list of spheres
    body_interactor bi;
    grid g;
    public:
        // TODO: timestep, gather+apply impulses, save/load state
        world (std::vector<sphere>);
        unsigned int count_spheres () { return spheres.size (); }
};

#endif // WORLD_H
