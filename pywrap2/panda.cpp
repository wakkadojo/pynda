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

    // begin sphere scope
    { 
        scope sphere_scope = class_<sphere> ("sphere")
            .def (init<> ())
            .def (init<double, double, double, vec3d, vec3d, vec3d> ())
            .def_readwrite ("m", &sphere::m)
            .def_readwrite ("r", &sphere::r)
            .def_readwrite ("I", &sphere::I)
            .def_readwrite ("x", &sphere::x)
            .def_readwrite ("v", &sphere::v)
            .def_readwrite ("q", &sphere::q)
            .def_readwrite ("w", &sphere::w)
            .def_readwrite ("flag", &sphere::flag)
        ;

        enum_<sphere::state> ("state")
            .value ("moving", sphere::state::moving)
            .value ("fixed", sphere::state::fixed)
            .value ("kill", sphere::state::kill)
        ; 
    }
    // end sphere scope

    // if these guys had return values we might have to do some memory handling fyi
    class_<io> ("io")
        .def ("save", &io::save)
        .staticmethod ("save")
        .def ("load", &io::load)
        .staticmethod ("load")
    ;
}
