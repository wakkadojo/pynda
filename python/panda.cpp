#include <world.hpp>
#include <body.hpp>
#include <io.hpp>
#include <linalg.hpp>
#include <boost/python.hpp>

using namespace boost::python;

BOOST_PYTHON_MODULE (panda)
{
    class_<world> ("world")
        .def ("add_sphere", &world::add_sphere)
        .def ("count_spheres", &world::count_spheres)
        .def ("step", &world::step)
    ;

    class_<vec3d> ("vec3d")
        .def ("__getitem__", &vec3d::get)
        .def ("__setitem__", &vec3d::set)
    ;

    class_<sphere> ("sphere")
    ;

    class_<io> ("io")
        .def ("save", &io::save)
        .def ("load", &io::load)
    ;
}
