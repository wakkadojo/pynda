#ifndef GRID_HPP
#define GRID_HPP

#include <string>
#include <fstream>
#include <vector>
#include <stdexcept>
#include "linalg.hpp"
#include "body.hpp"

// grid used to find particle neighbors
class grid
{
    unsigned int n_cells;
    // stores the occupying particle indexes in each cell
    std::vector<std::vector<unsigned int>> cells;
    // which cells should be searched for each given center cell
    std::vector<std::vector<unsigned int>> search_cells; 
    // cell numbers of the spheres
    std::vector<unsigned int> sphere_cells; 
    // indexes of all neighbors of each particle
    std::vector<std::vector<unsigned int>> neighbors;
    const static unsigned int neighbor_reserve = 10; // max neighbors we expect
    // the length of each dimension, centered at 0
    std::vector<double> box;
    // number of cells in each dimension
    std::vector<unsigned int> c;
    private:
        // find the cell number of the given sphere
        unsigned int get_sphere_cell (const sphere &);
        // sets which cells to look in for overlaps, should be called in constructor
        void set_search_cells ();
    public:
        grid ();
        grid (std::vector<unsigned int>, std::vector<double>);
        // adding spheres without recomputing entire grid
        void add (const sphere &, const unsigned int);
        // removing spheres without recomputing entire grid
        void remove (const sphere &, const unsigned int);
        // update the grid info for a given index
        void update (const sphere &, const unsigned int);
        // get neighbors of a sphere
        std::vector<unsigned int> get_neighbors (const unsigned int); 
        // make grid and neighbor list from scratch
        void complete_refresh (const std::vector<sphere> &);
};

#endif // GRID_HPP
