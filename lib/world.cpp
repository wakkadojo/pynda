#include "world.hpp"

// Grid
//

grid::grid ()
{
    // just set everything to something trivial
    c = { 1, 1, 1 };
    box = { 1.0, 1.0, 1.0 };
    n_cells = 0; // make sure it crashes later... we don't want these parameters
}

grid::grid (std::vector<unsigned int> c, std::vector<double> box)
{
    /**
     * 1. Only get search cells for cells with spheres, make warning note about that
     * 2. Eliminate having too many class variables -- only need them if we later decide to save grid between steps
     */
    this->c = c;
    this->box = box;

    n_cells = 1;
    for (unsigned int i=0; i<c.size (); ++i)
        n_cells *= c[i];
    
    // mark which cells should be searched for each cell
    cells = std::vector<std::vector<unsigned int>> (n_cells);
    set_search_cells ();
}

void grid::set_sphere_cells (const std::vector<sphere> & spheres) {
    sphere_cells = std::vector<unsigned int> (spheres.size ());
    cells.clear ();

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
            vec3d x = spheres[i].x; // make this guy so we can pass spheres as const
            cell += (unsigned int) (x[j]*c[j]/box[j])*mult;
            mult *= c[j];
        }
        // store the given sphere cell
        sphere_cells[i] = cell;
    }

    for (unsigned int i=0; i<spheres.size (); ++i)
    {
        // only add to cells if it was in our box
        unsigned int cell = sphere_cells[i];
        if (cell < n_cells)
            cells[cell].push_back (i); // add index i to the given cell 
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
        unsigned int center_cells [3] = {0, 0, 0}; // index of center cell in 3d
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
void grid::make_grid (const std::vector<sphere> & spheres)
{
    if (n_cells == 0)
        std::cerr << "WARNING: did not properly initialize grid" << std::endl;
   
    // update cell location for each sphere
    set_sphere_cells (spheres);

    // clear and initiate neighbor list
    neighbors = std::vector<std::vector<unsigned int>> (spheres.size ());

    // now go through the cells to construct the neighbor list
    // for now, this handles the monodisperse case
    for (unsigned int i=0; i<spheres.size (); ++i)
        for (auto adj_cell : search_cells[sphere_cells[i]])
            for (auto & neighbor : cells[adj_cell])
                if (neighbor != i) // dont add self to neighbor list
                    neighbors[i].push_back (neighbor);
}

std::vector<unsigned int> grid::get_neighbors (unsigned int i)
{
    return neighbors[i];
}

grid::~grid ()
{
    // vectors free themselves
}

// End Grid
//

// World
//

world::world () 
{
    c = { 6, 6, 6 };
    box = { 1.0, 1.0, 1.0 };
    t = 0;
    dt = 0.001; // RG/(100 U0)
    g = grid (c, box);
}

world::world (const vec3d cell_size, const body_interactor bi, const double dt) 
{
    box = { 1.0, 1.0, 1.0 };
    for (unsigned int i=0; i<cell_size.size (); ++i)
        c[i] = (unsigned int) (1.0/cell_size.get (i) + 0.5); // integer rounding
    this->bi = bi;
    this->dt = dt;
    t = 0;
    g = grid (this->c, this->box);
}

void world::clean () 
{
    std::vector<sphere> new_spheres;
    // we know we're going to do a lot of pushing...
    new_spheres.reserve (spheres.size ()); 
    for (auto & s : spheres)
        for (unsigned int i=0; i<box.size (); ++i)
            if (s.x[i] < 0 or s.x[i] > box[i])
                s.flag = sphere::state::kill;
    for (auto & s : spheres) 
        if (s.flag != sphere::state::kill)
            new_spheres.push_back (s);
    spheres = std::move (new_spheres);
}

void world::step () 
{
    // clean up / delete spheres
    clean ();

    // refresh grid grid
    g.make_grid (spheres);

    // Sphere interactions, TODO: continuous forces
    for (unsigned int i=0; i<spheres.size (); ++i)
        for (unsigned int j : g.get_neighbors (i))
            if (i < j)
                bi.interact (spheres[i], spheres[j]);

    // Interact with fixed objects
    for (auto & s : spheres) 
    {
        for (auto & b : bricks)
            bi.interact (b, s);
        // walls
        /*
        for (unsigned int i=0; i<box.size (); ++i)
        {
            if (s.x[i] < s.r)
            {
                s.x[i] = s.r;
                s.v[i] = s.v[i] < 0 ? -s.v[i] : s.v[i];
            }
            else if (s.x[i] > box[i] - s.r)
            {
                s.x[i] = box[i] - s.r;
                s.v[i] = s.v[i] > 0 ? -s.v[i] : s.v[i];
            }
        }
        */
    }

    // Position updating
    for (auto & s : spheres)
    {
        s.x = s.x + s.v*dt;
        s.q = s.q + s.w*dt;
    }

    // Update state variable
    t += dt;
}

// End World
