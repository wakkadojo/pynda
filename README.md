# panda: scientifically accurate rigid body simulations

Panda is a computationally lean and scientifically vetted 3D sphere physics engine written in C++ and bound to Python. The method has been validated against multiple granular flow experiments and continuum models, and is particularly efficient at high number density (high packing fraction). Also, this implementation not only utilizes of low computational complexity at every turn, but it has also been extensively profiled to remove multiple performance bottlenecks.

Dependencies: [Boost](http://www.boost.org/) development libraries

Last README update: 5/8/2015 (some functionaly may have changed)

## Basic simulation setup

A simulation is conducted inside of a "world" object. A world is a 3D space from -0.5 to 0.5 in all dimensions. Each world depends on 3 things:

1. Cell size -- the size of cells in each dimension. These cells should be approximately 3x the radius of the largest sphere
2. Body interactor -- an object containing the parameters for interactions between objects
3. Timestep -- the timestep should be about 0.01 R/U0 where R is the smallest sphere and U0 is the highest speed. Though, the timestep can be increased in most cases, the one above is gauranteed for 

Simulations can easily be made 2-dimensional by placing all spheres in a plane, and significant memory can be saved by not resolving the grid perpendicular to that plane.

## Controlling from python

Here we show how to load the simulation, and how to set up a basic simulation. For more complex examples, please browse the test_run directory and other source code, or send me an email.

#### Importing the simulation

The code currently compiles to the working source directory and does not install in your standard library path. Because of this, we need to tell Python to find the library. Suppose the pywrap directory is at /path/to/pywrap, then to load the library, use the following:

    import sys
    sys.append ('/path/to/pywrap') # add the pywrap directory to python's library search path
    import panda

Now you have it! Note: there are separate paths for Python 2 (pywrap2) and Python 3 (pywrap).

#### A simple simulation: colliding two spheres
Once you have the simulation loaded, you need to create a world object, create some spheres, and let it run.

First, lets make some spheres

    r = 0.05          # sphere radius
    m = 4*pi*r**3/3.0 # sphere mass
    u0 = 1.0          # initial speed
    # sphere arguments: r, m, I, position, velocity, angular velocity
    # vec3d is part of an optimized vector library
    s1 = panda.sphere (r, m, I, panda.vec3d (-0.25,  r/4.0, 0.0), panda.vec3d ( u0, 0.0, 0.0), panda.vec3d ())
    s2 = panda.sphere (r, m, I, panda.vec3d ( 0.25, -r/4.0, 0.0), panda.vec3d (-u0, 0.0, 0.0), panda.vec3d ())

Next, we make the world and add the spheres

    bi = panda.interactor (0.4, 0.7) # arguments are coefficient of friction, coefficient of restitution
    cell_size = panda.vec3d (3.0*r, 3.0*r, 3.0*r) # arguments are the grid box sizes in eac dimension
    dt = 0.01*r/u0
    w = panda.world (cell_size, bi, dt)
    w.add_sphere (s1)
    w.add_sphere (s2)

Finally we can run it and see where the spheres are going!

    for _ in range (50):
        w.step (25) # execute 25 timesteps
        for i in range (w.num_spheres ()):
            s = w.get_sphere (i)
            print ('Sphere ' + str (i+1) + ': ' + str (s.x[0]) + ' ' + str (s.x[1]) + ' ' + str (x[2]))
        print ()

## Complete python reference list

    world
        .__init__ () # default constructor
        .__init__ (vec3d cell_size, interactor body_interactor, double timestep)
        .save (filename) # save the simulation state to "filename"
        .load (filename) # load simulation state from "filename"
        .add_sphere (sphere s) # add a sphere to the world
        .num_spheres () # return the number of spheres in world (integer)
        .get_spphere (int index) # return sphere number index to python
        .add_brick (brick b) # add a brick to the world
        .num_brigs () # return the number of bricks in world (integer)
        .get_brick (int index) # return brick number index to python
        .step () # execute a single timestep
        .step (int n) # execute n timesteps
        
    vec3d
        .__init__ () # default constructor: (0, 0, 0)
        .__init__ (int x, int y, int z) # initialize vector as (x, y, z)
        .__getitem__ (int i) # get item i by calling a = v[i]
        .__setitem__ (int i) # set item i by calling v[i] = a
        
    interactor
        .__init__ () # default constructor with friction 0.2 and restitution 0.9
        .__init__ (float mu, float cor) # body interactor with friction mu and restitution cor
        
    brick
        .__init__ () # default constructor
        .__init__ (vec3d x, vec3d L) # place a brick (cuboid) at x with side lengths L
        .x # vec3d: the brick's position
        .L # vec3d: the brick's side lengths
        
    sphere
        .__init__ () # default constructor
        # radius, mass, moment of inertia, position, velocity, and angular velocity
        .__init__ (float r, float m, float I, vec3d x, vec3d v, vec3d w) 
        .r # float: sphere radius
        .m # float: sphere mass
        .I # float: sphere moment of inertia
        .x # vec3d: sphere position
        .v # vec3d: sphere velocity
        .w # vec3d: sphere angular velocity (spin rate)
        .flag # sphere state: see below
        # Below sphere state enum is static
        .state
            .moving # sphere is moving, interacts with other bodies normally
            .fixed # sphere is fixed, can affect other bodies but can't be affected by other bodies
            .kill # sphere is marked for deletion

## Controlling from C++
Largely the same as python, but have access to a few extra functions that haven't been wrapped

## Furture features
* Stress tensor measurements
* Scalable parallel simulations
* Continuous forces and custom sphere interactions
* (less likely) Sphere bundles and custom shapes

## References
1. [N. Guttenberg, Phys. Rev. E **83** (2011)](http://journals.aps.org/pre/abstract/10.1103/PhysRevE.83.051306)
2. [J. Ellowitz, H. Turlier, N. Guttenberg, W. W. Zhang, S. R. Nagel, Phys. Rev. Lett. **111** (2013)](http://journals.aps.org/prl/abstract/10.1103/PhysRevLett.111.168001)
