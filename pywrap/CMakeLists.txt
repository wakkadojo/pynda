project (PANDA)

find_package (PythonLibs 3 REQUIRED)
find_package (Boost 1.40 COMPONENTS python3 REQUIRED)
include_directories (${PYTHON_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})

add_library (pynda SHARED pynda.cpp)
set_target_properties (pynda PROPERTIES PREFIX "")
target_link_libraries (pynda LINK_PUBLIC panda ${PYTHON_LIBRARIES} ${Boost_LIBRARIES})
