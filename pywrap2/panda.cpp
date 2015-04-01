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
        .def ("num_spheres", &world::num_spheres)
        .def ("get_sphere", &world::get_sphere)
        .def ("step", &world::step)
    ;

    class_<vec3d> ("vec3d")
        .def (init<>())
        .def (init<double, double, double>())
        .def ("__getitem__", &vec3d::get)
        .def ("__setitem__", &vec3d::set)
    ;

    class_<sphere> ("sphere")
        .def ("init", &sphere::init)
        .def_readonly ("m", &sphere::m)
        .def_readonly ("r", &sphere::r)
        .def_readonly ("I", &sphere::I)
        .def_readonly ("x", &sphere::x)
        .def_readonly ("v", &sphere::v)
        .def_readonly ("q", &sphere::q)
        .def_readonly ("w", &sphere::w)
    ;

    // if these guys had return values we might have to do some memory handling fyi
    class_<io> ("io")
        .def ("save", &io::save)
        .staticmethod ("save")
        .def ("load", &io::load)
        .staticmethod ("load")
    ;
}
