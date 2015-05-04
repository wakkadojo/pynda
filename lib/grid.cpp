#include "grid.hpp"

grid::grid ()
{
    // just set everything to something trivial
    c = { 1, 1, 1 };
    n_cells = 0; // make sure it crashes later... we don't want these parameters
}

grid::grid (std::vector<unsigned int> c, std::vector<double> min_box, std::vector<double> max_box)
{
    /**
     * 1. Only get search cells for cells with spheres, make warning note about that
     * 2. Eliminate having too many class variables -- only need them if we later decide to save grid between steps
     */
    this->c = c;
    this->min_box = min_box;
    this->max_box = max_box;

    n_cells = 1;
    for (unsigned int i=0; i<c.size (); ++i)
        n_cells *= c[i];
    
    // mark which cells should be searched for each cell
    set_search_cells ();

    // need initial grid refresh
    complete_refresh (std::vector<sphere> ());
}

void grid::add (const sphere & s, const unsigned int index)
{
    if (index != sphere_cells.size ())
        throw std::out_of_range ("bad book-keeping: number of spheres does not match number in grid");
    unsigned int cell = get_sphere_cell (s);
    sphere_cells.push_back (cell);
    // now go through the cells to construct the neighbor list
    neighbors.push_back (std::vector<unsigned int> ());
    neighbors.reserve (neighbor_reserve);
    for (const unsigned int & adj_cell : search_cells[cell])
        for (const unsigned int & neighbor : cells[adj_cell])
        {
            // don't need to check if neighber == index since index not in grid yet
            neighbors[index].push_back (neighbor);
            neighbors[neighbor].push_back (index);
        }
    cells[cell].push_back (index);
}

void grid::remove (const sphere & s, const unsigned int index)
{
    unsigned int cell = sphere_cells[index];
    helpers::remove_element (cells[cell], index);
    sphere_cells.erase (sphere_cells.begin () + index); // pops everyone back down one
    // need to update all indexes larger than or equal to index
    for (unsigned int i=index; i<sphere_cells.size (); ++i)
        for (unsigned int & j : cells[sphere_cells[i]])
            if (j == i+1) // correct everyone for the pop that happened above
            {
                j--;
                break;
            }

    // clean up neighbors by decrementing everyone above and removing our target
    // use fact that a ~ b --> b ~ a, search nearby guys and update neighbor info
    for (const unsigned int & neighbor : neighbors[index])
        helpers::remove_element (neighbors[neighbor], index);
    neighbors.erase (neighbors.begin () + index); // pops everyone down one
    // update all indexes of everyone accounting for the incoming shift (diff from above)
    for (std::vector<unsigned int> & neighbor_vec : neighbors)
        for (unsigned int & j : neighbor_vec)
            if (j > index)
                j--;
}

void grid::update (const sphere & s, const unsigned int index)
{
    unsigned int old_cell = sphere_cells[index];
    unsigned int cell = get_sphere_cell (s);
    if (cell != old_cell)
    {
        // remove old info
        helpers::remove_element (cells[old_cell], index);
        for (const unsigned int & neighbor : neighbors[index])
            helpers::remove_element (neighbors[neighbor], index);
        // update new info
        sphere_cells[index] = cell;
        cells[cell].push_back (index);
        neighbors[index].clear ();
        for (const unsigned int & adj_cell : search_cells[cell])
            for (const unsigned int & neighbor : cells[adj_cell])
                if (neighbor != index)
                {
                    neighbors[index].push_back (neighbor);
                    neighbors[neighbor].push_back (index);
                }
    }
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
        if (x[i] < min_box[i] or x[i] > max_box[i])
            throw std::out_of_range ("sphere position is outside box");
        cell += (unsigned int) ((x[i] - min_box[i])*c[i]/(max_box[i]-min_box[i]))*mult;
        mult *= c[i];
    }
    return cell;
}

// TODO deprecated
void grid::complete_refresh (const std::vector<sphere> & spheres) {

    sphere_cells = std::vector<unsigned int> (spheres.size ());
    cells = std::vector<std::vector<unsigned int>> (n_cells);
    neighbors = std::vector<std::vector<unsigned int>> ();

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
        cells[cell].push_back (i); // add index i to the given cell 
    }

    for (unsigned int i=0; i<neighbors.size (); ++i)
    {
        neighbors[i].reserve (neighbor_reserve);
        for (unsigned int adj_cell : search_cells[sphere_cells[i]])
            for (unsigned int neighbor : cells[adj_cell])
                if (neighbor != i)
                    neighbors[i].push_back (neighbor);
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

std::vector<unsigned int> grid::get_neighbors (const unsigned int i)
{
    return neighbors[i];
}
