#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include "linalg.h"
#include "constants.h"
#include "body.h"

// grid used to find particle neighbors
class grid
{
	unsigned int n_cells;
	std::vector< std::vector<unsigned int> > cells;   // stores the occupying particle indexes in each cell
	std::vector< std::vector<unsigned int> > neighbors; // stores the neighbors of each particle
	double box[d];         // the length of each dimension, centered at 0
	unsigned int c[d];     // number of cells in each dimension
	private:
		void clear_cells ();     // empties each cell
		void clear_neighbors (); // empties each neighbor list
		void make_grid (std::vector<sphere>); // make grid and neighbor list
	public:
		grid ();
		grid (unsigned int *, double *, std::vector<sphere>);
		~grid ();
		// TODO: once simulation is up and running, make new local cell 
		// searching algorithm
		std::vector<unsigned int> & get_neighbors (unsigned int); // get neighbors of a sphere
		std::vector<unsigned int> & get_spheres_in_cell (unsigned int); // return spheres in cell
};

// Handles tracking spheres and finding neighbors etc
class world
{
	std::vector<sphere> spheres; // list of spheres
	//body_interactor bi;
	grid g;
	public:
		world (std::vector<sphere>);
};

#endif // WORLD_H
