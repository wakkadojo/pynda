#include "world.h"

// Grid
//

grid::grid ()
{
    // just set everything to something trivial
    c = { 1, 1, 1 };
    box = { 1.0, 1.0, 1.0 };
    n_cells = 0; // make sure it crashes later... we don't want these parameters
}

grid::grid (std::vector<unsigned int> &c, std::vector<double> &box, const std::vector<sphere>&spheres)
{
    this->c = c;
    this->box = box;
    // allocate 1 spheres just so that the memory is SOMETHING
    neighbors = std::vector<std::vector<unsigned int>> (spheres.size ());

    n_cells = 1;
    for (unsigned int i=0; i<c.size (); ++i)
        n_cells *= c[i];
    cells = std::vector<std::vector<unsigned int>> (n_cells);
    
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

void grid::set_sphere_cells (std::vector<sphere> spheres) {
    sphere_cells = std::vector<unsigned int> (spheres.size ());
    // iterate through all spheres and place them on the cell grid
    for (unsigned int i=0; i<spheres.size (); ++i)
    {
        // the box lengths are box, and extend from -l/2 to l/2 (eg)
        unsigned int cell = 0;
        unsigned int mult = 1;
        for (unsigned int j=0; j<c.size (); ++j)
        {
            // if the sphere is in the box, this cell number will be in 
            // the range [0, n_cells]
            cell += (unsigned int) (spheres[i].x[j]*c[j]/box[j])*mult;
            mult *= c[j];
        }
        // only add to cells if it was in our box
        if (cell < n_cells)
            cells[cell].push_back (i); // add index i to the given cell 
        // store the given sphere cell
        sphere_cells.push_back (cell);
    }
}

void grid::set_search_cells () {
    search_cells = std::vector<std::vector<unsigned int>> (n_cells);
    for (unsigned int i=0; i<search_cells.size (); ++i)
    {
        // --------------------
        // make a list of all adjacent cells
        // For now just hardcode d = 2/3 in with many nested for loops. There
        // should be a recursive algorithm that finds 3 cells and calls 3 more
        // of itself etc to generate the cell list. But perhaps implement this
        // after the simulation is up and working.
        unsigned int center_cells [3]; // index of center cell in 3d
        unsigned int mult = 1;
        for (unsigned int j=0; j<c.size (); ++j)
        {
            center_cells[j] = (i/mult)%c[j]; // index in each dimension
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
                    if (scj>=0 && sck>=0 && scl>=0 &&
                        (unsigned int) scj<c[0] &&
                        (unsigned int) sck<c[1] &&
                        (unsigned int) scl<c[2])
                        // store what each cell was in each dimension
                        search_cells[i].push_back (scj+sck*c[0]+scl*c[0]*c[1]);
                }
    }
}

// TODO: Modularize, save "search cells", clean up, test
void grid::make_grid (std::vector<sphere> spheres)
{
    if (n_cells == 0)
        std::cerr << "WARNING: did not properly initialize grid" << std::endl;

    // clean up before doing anything
    clear_neighbors ();
   
    // update cell location for each sphere
    set_sphere_cells (spheres);
    // update which cells should be searched for each cell
    set_search_cells ();

    // now go through the cells to construct the neighbor list
    // for now, this handles the monodisperse case
    for (unsigned int i=0; i<spheres.size (); ++i)
        for (auto adj_cell : search_cells[sphere_cells[i]])
            for (auto & neighbor : cells[adj_cell])
                if (neighbor != i) // dont add self to neighbor list
                    neighbors[i].push_back (neighbor);
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
    std::vector<unsigned int> c = { 6, 6, 6 };
    std::vector<double> box = { 1.0, 1.0, 1.0 };
    g = grid (c, box, spheres);
    std::vector<unsigned int> neighbs = g.get_neighbors (2);
    for (auto & it : neighbs)
        std::cout << it << std::endl;
}

// End World
//
