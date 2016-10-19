cmake_minimum_required(VERSION 2.8)
include(ExternalProject)

SET(CMAKE_CXX_FLAGS "-std=c++0x -O2")
SET(ENV{BOOST_ROOT} "/usr/lib")

#substitute your version number
find_package(Boost 1.56.0 COMPONENTS python)
INCLUDE_DIRECTORIES("${Boost_INCLUDE_DIRS}")

find_package(PythonLibs 2.7 REQUIRED )
INCLUDE_DIRECTORIES("${PYTHON_INCLUDE_DIRS}")

ADD_LIBRARY(spatial_index SHARED spatial.cpp)
TARGET_LINK_LIBRARIES(spatial_index ${Boost_LIBRARIES})
SET_TARGET_PROPERTIES(spatial_index PROPERTIES PREFIX "" )
