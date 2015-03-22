#include "world.h"

// Grid
//

grid::grid ()
{
    // just set everything to 0
    n_cells = 0;
    for (unsigned int i=0; i<d; ++i)
    {
        c[i] = 0;
        box[i] = 0.0;
    }
}

grid::grid (std::vector<unsigned int> &c, std::vector<double> &box, const std::vector<sphere>&spheres)
{
    // allocate 1 spheres just so that the memory is SOMETHING
    for (unsigned int i=0; i<spheres.size (); ++i)
        neighbors.push_back (std::vector<unsigned int> ());
    n_cells = 1;
    for (unsigned int i=0; i<c.size (); ++i)
    {
        this->c[i]   = c[i];
        this->box[i] = box[i];
        n_cells *= c[i];
    }
    for (unsigned int i=0; i<n_cells; ++i)
        cells.push_back (std::vector<unsigned int> ());
    make_grid (spheres);
}

void grid::clear_cells ()
{
    for (unsigned int i=0; i<n_cells; ++i)
        cells[i].clear ();
}

void grid::clear_neighbors ()
{
    for (unsigned int i=0; i<neighbors.size (); ++i)
        neighbors[i].clear ();
}

// TODO: Modularize, save "search cells", clean up, test
void grid::make_grid (std::vector<sphere> spheres)
{
    // clean up before doing anything
    clear_neighbors ();
    // create a list that stores the given cell of each sphere
    std::vector<unsigned int> sphere_cells;
    // This list will contain the cells searched by any given sphere (temp arr.)
    unsigned int search_cells [d*d*d]; 
    
    // now iterate through all spheres and place them on the cell grid
    for (unsigned int i=0; i<spheres.size (); ++i)
    {
        // the box lengths are box, and extend from -l/2 to l/2 (eg)
        unsigned int cell = 0;
        unsigned int mult = 1;
        for (unsigned int j=0; j<c.size (); ++j)
        {
            // if the sphere is in the box, this cell number will be in 
            // the range [0, n_cells]
            cell += (unsigned int) (spheres[i].x.get (j)*c[j]/box[j])*mult;
            mult *= c[j];
        }
        // only add to cells if it was in our box
        if (cell < n_cells)
            cells[cell].push_back (i); // add index i to the given cell 
        // store the given sphere cell
        sphere_cells.push_back (cell);
    }
    
    // now go through the cells to construct the neighbor list
    // for now, this handles the monodisperse case
    for (unsigned int i=0; i<spheres.size (); ++i)
    {
        std::cout << i << std::endl;
        unsigned int s_cell = sphere_cells[i];

        // --------------------
        // make a list of all adjacent cells
        // For now just hardcode d = 2/3 in with many nested for loops. There
        // should be a recursive algorithm that finds 3 cells and calls 3 more
        // of itself etc to generate the cell list. But perhaps implement this
        // after the simulation is up and working.
        unsigned int center_cells [d]; // implemented same way as Nich's code
        unsigned int mult = 1;
        for (unsigned int j=0; j<c.size (); ++j)
        {
            center_cells[j] = (s_cell/mult)%c[j]; // index in each dimension
            mult *= c[j];
        }
        // one grain size search distance, hardcode 3d
        for (int j=0; j<=2; ++j)
            for (int k=0; k<=2; ++k)
                for (int l=0; l<=2; ++l)
                {
                    // grab the search cells -- move one way in each dimension
                    int scj = center_cells[0] + j - 1; 
                    int sck = center_cells[1] + k - 1; 
                    int scl = center_cells[2] + l - 1; 
                    // check bounds here, if over the edge then mark the search
                    // cell by a value that will be ignored later
                    if (scj>=0   && sck>=0   && scl>=0 &&
                        (unsigned int) scj<c[0] &&
                        (unsigned int) sck<c[1] &&
                        (unsigned int) scl<c[2])
                        // store what each cell was in each dimension
                        search_cells[j + k*d + l*d*d] = scj + sck*c[0] + scl*c[0]*c[1];
                    else
                        // flag queried cell out of bounds
                        search_cells[j + k*d + l*d*d] = n_cells;
                }
        // done dentifying adjacent cells
        // --------------------

        // go through all adjacent cells and compile neighbor list
        for (unsigned int j=0; j<3*3*3; ++j)
        {
            unsigned int adj_cell = search_cells[j];
            if (s_cell < n_cells && adj_cell < n_cells)
                for (auto & neighbor : cells[adj_cell])
                    if (neighbor != i) // dont add self to neighbor list
                        neighbors[i].push_back (neighbor);
        }
    }
}

std::vector<unsigned int> & grid::get_neighbors (unsigned int i)
{
    return neighbors[i];
}

std::vector<unsigned int> & grid::get_spheres_in_cell (unsigned int i)
{
    return cells[i];
}

grid::~grid ()
{
    // vectors free themselves
}

// End Grid
//

// World
//

world::world (std::vector<sphere> spheres)
{
    this->spheres = spheres;
    std::cout << "lets get started" << std::endl;
    //std::vector<unsigned int> c = { 6, 6, 6 };
    //std::vector<double> box = { 1.0, 1.0, 1.0 };
    //g = grid (c, box, spheres);
    //std::vector<unsigned int> neighbs = g.get_neighbors (0);
    //for (auto & it : neighbs)
    //    std::cout << it << std::endl;
}

// End World
//
