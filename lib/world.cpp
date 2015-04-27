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
    set_search_cells ();
}

void grid::add (const sphere & s, const unsigned int index)
{
    if (index != sphere_cells.size () + 1)
        throw std::out_of_range ("bad book-keeping: number of spheres does not match number in grid");
    unsigned int cell = get_sphere_cell (s);
    sphere_cells.push_back (cell);
    cells[cell].push_back (index);
    neighbors.push_back (std::vector<unsigned int> ());
    for (auto adj_cell : search_cells[cell])
        for (auto & neighbor : cells[adj_cell])
            if (neighbor != index) // dont add self to neighbor list
            {
                neighbors[index].push_back (neighbor);
                neighbors[neighbor].push_back (index);
            }
}

void grid::remove (const unsigned int index)
{
    
}

void grid::update (const unsigned int index)
{

}

unsigned int grid::get_sphere_cell (const sphere & s)
{
    // if the sphere is in the box, this cell number will be in 
    // the range [0, n_cells]
    unsigned int cell = 0;
    unsigned int mult = 1;
    for (unsigned int i=0; i<c.size (); ++i)
    {
        vec3d x = s.x; // so can use non-const ref
        if (x[i] < 0 or x[i] > box[i])
            throw std::out_of_range ("sphere position is outside box");
        cell += (unsigned int) (x[i]*c[i]/box[i])*mult;
        mult *= c[i];
    }
    return cell;
}

void grid::set_sphere_cells (const std::vector<sphere> & spheres) {
    // In this function we do not add spheres to the search path if they are marked
    // to be deleted. This is one step so a deleted/ingorable sphere won't interact
    // with the rest of the spheres and also won't give errors for out of bounds


    sphere_cells = std::vector<unsigned int> (spheres.size ());
    cells = std::vector<std::vector<unsigned int>> (n_cells);

    // iterate through all spheres and place them on the cell grid
    for (unsigned int i=0; i<spheres.size (); ++i)
    {
        // store the given sphere cell
        sphere_cells[i] = get_sphere_cell (spheres[i]);
    }

    for (unsigned int i=0; i<spheres.size (); ++i)
    {
        // only add to cells if it was in our box
        unsigned int cell = sphere_cells[i];
        if (spheres[i].flag != sphere::state::kill) // only look if not deletable
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

void grid::complete_refresh(const std::vector<sphere> & spheres)
{
    // in this function, like in set_sphere_cells, we have a check to make sure that
    // a sphere that's going to be deleted/killed does not have a chance to interact
    // with the other spheres, or give an error for having a bad cell number.

    if (n_cells == 0)
        std::cerr << "WARNING: did not properly initialize grid" << std::endl;
   
    // update cell location for each sphere
    set_sphere_cells (spheres);

    // clear and initiate neighbor list
    neighbors = std::vector<std::vector<unsigned int>> (spheres.size ());

    // now go through the cells to construct the neighbor list
    // for now, this handles the monodisperse case
    for (unsigned int i=0; i<spheres.size (); ++i)
        if (spheres[i].flag != sphere::state::kill) // don't procede for deletable
            for (auto adj_cell : search_cells[sphere_cells[i]])
                for (auto & neighbor : cells[adj_cell])
                    if (neighbor != i) // dont add self to neighbor list
                        neighbors[i].push_back (neighbor);
}

std::vector<unsigned int> grid::get_neighbors (const unsigned int i)
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
    grid_step_counter = 0;
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
    grid_step_counter = 0;
}

void world::update_flags () 
{
    for (auto & s : spheres)
        for (unsigned int i=0; i<box.size (); ++i)
            if (s.x[i] < 0 or s.x[i] > box[i])
                s.flag = sphere::state::kill;
}

void world::clean ()
{
    std::vector<sphere> new_spheres;
    // we know we're going to do a lot of pushing...
    new_spheres.reserve (spheres.size ()); 
    for (auto & s : spheres) 
        if (s.flag != sphere::state::kill)
            new_spheres.push_back (s);
    spheres = std::move (new_spheres);
}

void world::add_sphere (sphere s) 
{ 
    spheres.push_back (s); 
}

void world::add_brick (brick b) 
{ 
    bricks.push_back (b); 
}

unsigned int world::num_spheres () 
{ 
    return spheres.size (); 
}

unsigned int world::num_bricks () 
{ 
    return bricks.size (); 
}

sphere world::get_sphere (unsigned int i) 
{
    if (i < spheres.size ())
        return spheres[i]; 
    throw std::out_of_range ("requested sphere outside of range");
}

brick world::get_brick (unsigned int i) 
{
    if (i < bricks.size ())
        return bricks[i]; 
    throw std::out_of_range ("requested brick outside of range");
}

void world::step () 
{
    // Sphere removal naturally should be done when the grid gets updated

    // update sphere flags
    update_flags ();

    // refresh grid grid
    if (grid_step_counter++ > grid_update_steps)
    {
        clean (); // THIS IS WHERE SPHERES ARE REMOVED
        grid_step_counter = 0;
        g.complete_refresh (spheres); // rebuild the grid based on cleaned up spheres
    }

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

void world::step (unsigned int n)
{
    for (unsigned int i=0; i<n; ++i)
        step ();
}

// End World
