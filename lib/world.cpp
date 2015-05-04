#include "world.hpp"

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

void world::save (std::string filename)
{
    std::ofstream os (filename);
    boost::archive::binary_oarchive oa (os);
    oa << *this;
}

void world::load (std::string filename)
{
    std::ifstream is (filename);
    boost::archive::binary_iarchive ia (is);
    ia >> *this;
    // regenerate grid
    g.complete_refresh (spheres); 
}

void world::update_flags () 
{
    for (sphere & s : spheres)
        for (unsigned int i=0; i<box.size (); ++i)
            if (s.x[i] < 0 or s.x[i] > box[i])
                s.flag = sphere::state::kill;
}

void world::clean ()
{
    if (not spheres.empty ())
        for (int i=spheres.size ()-1; i>=0; --i)
        {
            if (spheres[i].flag == sphere::state::kill)
            {
                // have to remove from grid before remove from spheres
                g.remove (spheres[i], i);
                spheres.erase (spheres.begin () + i);
            }
        }
}

void world::add_sphere (sphere s) 
{ 
    g.add (s, spheres.size ());
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
    // clean up and remove rogue spheres
    clean (); 

    for (unsigned int i=0; i<spheres.size (); ++i)
        g.update (spheres[i], i);

    // Sphere interactions, TODO: continuous forces
    for (unsigned int i=0; i<spheres.size (); ++i)
        for (unsigned int j : g.get_neighbors (i))
            if (i < j)
                bi.interact (spheres[i], spheres[j]);

    // Interact with fixed objects
    for (brick & b : bricks)
        for (sphere & s : spheres) 
            bi.interact (b, s);

    // Position updating
    for (sphere & s : spheres)
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
