project (PANDA)

find_package (Boost 1.40 COMPONENTS serialization REQUIRED)
include_directories (${Boost_INCLUDE_DIRS})

add_library (panda SHARED body.cpp grid.cpp interactor.cpp linalg.cpp world.cpp)

target_link_libraries (panda ${Boost_LIBRARIES})
