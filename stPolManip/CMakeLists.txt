# CMakeLists.txt for event package. It creates a library with dictionary and a main program
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(stPolManip)

# You need to tell CMake where to find the ROOT installation. This can be done in a number of ways:
#   - ROOT built with classic configure/make use the provided $ROOTSYS/etc/cmake/FindROOT.cmake
#   - ROOT built with CMake. Add in CMAKE_PREFIX_PATH the installation prefix for ROOT
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})

#---Locate the ROOT package and defines a number of variables (e.g. ROOT_INCLUDE_DIRS)
find_package(ROOT REQUIRED COMPONENTS RIO Net Core MathCore Hist)

#---Define useful ROOT functions and macros (e.g. ROOT_GENERATE_DICTIONARY)
include(${ROOT_USE_FILE})
include_directories(stPolManip)

#---Create  a main program using the library
set(SOURCES src/main.cpp src/stPolManip.cpp src/protosInput.cpp)
set(HEADERS
   ${CMAKE_CURRENT_SOURCE_DIR}/stPolManip/stPolManip.h
   ${CMAKE_CURRENT_SOURCE_DIR}/stPolManip/protosInput.h

)
add_executable(stPolManip ${SOURCES} ${HEADERS})
target_link_libraries(stPolManip ${ROOT_LIBRARIES})
