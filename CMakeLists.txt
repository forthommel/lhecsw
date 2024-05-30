cmake_minimum_required(VERSION 3.16)

project(lhecsw)
set(lhecswlibs)

set(CMAKE_CXX_STANDARD 20)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)  # release build by default
endif()
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake $ENV{DD4hep_DIR}/cmake)
set(CMAKE_CXX_FLAGS_RELEASE "-Wall -Wextra -O2 -pedantic")

include(lhecswBuildUtils)
include_directories(${PROJECT_SOURCE_DIR})

find_package(DD4hep REQUIRED)
include(${DD4hep_DIR}/cmake/DD4hep.cmake)
include(${DD4hep_DIR}/cmake/DD4hepBuild.cmake)

add_subdirectory(Common)
add_subdirectory(DataFormats)
add_subdirectory(Generator)
add_subdirectory(Geometry)

dd4hep_configure_output()
dd4hep_set_compiler_flags()
dd4hep_add_plugin(lhecsw SOURCES src/*.cc
  USES DD4hep::DDCore DD4hep::DDRec
       ROOT::Core ROOT::Geom ROOT::GenVector ROOT::MathCore
       ${lhecswlibs})

install(TARGETS lhecsw LIBRARY DESTINATION lib)
#install(DIRECTORY compact DESTINATION examples/ClientTests)

dd4hep_configure_scripts(lhecsw DEFAULT_SETUP WITH_TESTS)
