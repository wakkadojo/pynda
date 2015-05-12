project (PANDA)

find_package (PythonLibs REQUIRED)

if (${PYTHONLIBS_VERSION_STRING} VERSION_LESS 3) # if python < 3 use boost python2
    message (STATUS "Python ${PYTHONLIBS_VERSION_STRING} found, using python 2 bindings")
    find_package (Boost 1.40 COMPONENTS python REQUIRED) 
else () # if python 3, use boost python3
    message (STATUS "Python ${PYTHONLIBS_VERSION_STRING} found, using python 3 bindings")
    find_package (Boost 1.40 COMPONENTS python3 REQUIRED)
endif ()

# we have boost and python if we've made it this far
include_directories (${PYTHON_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS}) 

add_library (pynda SHARED pynda.cpp)
set_target_properties (pynda PROPERTIES PREFIX "")
target_link_libraries (pynda LINK_PUBLIC panda ${PYTHON_LIBRARIES} ${Boost_LIBRARIES})
