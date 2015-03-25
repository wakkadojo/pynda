#include "io.hpp"

void io::save (const world & w, std::string filename) 
{
    std::ofstream os (filename);
    boost::archive::binary_oarchive oa (os);
    oa & w;
}

world io::load (std::string filename) 
{
    std::ifstream is (filename);
    boost::archive::binary_iarchive ia (is);
    world w;
    ia & w;
    return w;
}
