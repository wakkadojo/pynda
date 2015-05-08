## panda: scientifically accurate rigid body simulations

Panda is a very computationally lean sphere physics engine in 3D written in C++ and bound to Python. The method has been validated against multiple granular flow experiments and continuum simulations, and has been shown to be scientifically accurate. Also, this implementation not only utilizes of low computational complexity at every turn, but it has also been extensively profiled to remove multiple performance bottlenecks.

Dependencies: [Boost](http://www.boost.org/) development libraries
Last README update: 5/8/2015 (some functionaly may have changed)

### Basic simulation setup

A simulation is conducted inside of a "world" object. A world is a 3D space from -0.5 to 0.5 in all dimensions. Each world depends on 3 things:

1. Cell size -- the size of cells in each dimension. These cells should be approximately 3x the radius of the largest sphere
2. Body interactor -- an object containing the parameters for interactions between objects
3. Timestep -- the timestep should be about 0.01 R/U0 where R is the smallest sphere and U0 is the highest speed. Though, the timestep can be increased in most cases, the one above is gauranteed for 

Simulations can easily be made 2-dimensional by placing all spheres in a plane, and significant memory can be saved by not resolving the grid perpendicular to that plane.

### Controlling from python

Here we show how to load the simulation, and how to set up a basic simulation. For more complex examples, please browse the test_run directory and other source code, or email the author.

#### Importing the simulation

The code currently compiles to the working source directory and does not install in your standard library path. Because of this, we need to tell Python to find the library. Suppose the pywrap directory is at /path/to/pywrap, then to load the library, use the following:

    import sys
    sys.append ('/path/to/pywrap') # add the pywrap directory to python's library search path
    import panda

Now you have it! Note: there are separate paths for Python 2 (pywrap2) and Python 3 (pywrap).

#### A simple simulation: colliding two spheres
Once you have the simulation loaded, you need to create a world object, create some spheres, and let it run.

First, lets make some spheres

    r = 0.05 # basic size
    m = 4*pi*r**3/3.0 # sphere mass
    u0 = 1.0 # speed
    # sphere arguments: r, m, I, position, velocity, angular velocity
    s1 = panda.sphere (r, m, I, panda.vec3d (-0.25,  r/4.0, 0.0), panda.vec3d ( u0, 0.0, 0.0), panda.vec3d ())
    s2 = panda.sphere (r, m, I, panda.vec3d ( 0.25, -r/4.0, 0.0), panda.vec3d (-u0, 0.0, 0.0), panda.vec3d ())

Now, we make the world and add the spheres

    bi = panda.interactor (0.4, 0.7) # arguments are coefficient of friction, coefficient of restitution
    cell_size = panda.vec3d (3.0*r, 3.0*r, 3.0*r)
    dt = 0.01*r/u0
    w = panda.world (cell_size, bi, dt)
    w.add_sphere (s1)
    w.add_sphere (s2)

Now we can run it and see where the spheres are going!

    for _ in range (100):
        w.step (10)
        for i in range (w.num_spheres ()):
            s = w.get_sphere (i)
            print ('Sphere ' + str (i+1) + ': ' + str (s.x[0]) + ' ' + str (s.x[1]) + ' ' + str (x[2]))
        print ()

### Controlling from C++
Largely the same as python, but have access to a few extra functions that haven't been wrapped

### Complete python reference list
Coming soon

### Furture features
* Stress tensor measurements
* Scalable parallel simulations
* Continuous forces and custom sphere interactions
* (less likely) Sphere bundles and different shapes

### References
1. [N. Guttenberg, Phys. Rev. E **83** (2011)](http://journals.aps.org/pre/abstract/10.1103/PhysRevE.83.051306)
2. [J. Ellowitz, H. Turlier, N. Guttenberg, W. W. Zhang, S. R. Nagel, Phys. Rev. Lett. **111** (2013)](http://journals.aps.org/prl/abstract/10.1103/PhysRevLett.111.168001)
