project(extractDepAction)
cmake_minimum_required(VERSION 2.7 FATAL_ERROR)

set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

if(CMAKE_BUILD_TYPE MATCHES Debug)
    SET( CMAKE_CXX_FLAGS  "-fopenmp -w -pthread -msse3 -funroll-loops -std=c++0x -O0" )
else()
    SET( CMAKE_CXX_FLAGS  "-fopenmp -w -pthread -msse3 -funroll-loops -std=c++0x -O3" )
endif()

add_subdirectory(src)
