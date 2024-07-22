cmake_minimum_required(VERSION 3.26)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O2 -pedantic")
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake $ENV{DD4hep_DIR}/cmake)

project(lhecsw)
set(lhecswlibs)
include(lhecswEnvironmentCheck)
include(lhecswBuildUtils)

find_package(podio REQUIRED)
find_package(Gaudi REQUIRED)
find_package(EDM4HEP REQUIRED)
find_package(k4FWCore REQUIRED)

find_package(DD4hep REQUIRED)
include(${DD4hep_DIR}/cmake/DD4hep.cmake)
include(${DD4hep_DIR}/cmake/DD4hepBuild.cmake)
dd4hep_configure_output()

include_directories(${PROJECT_SOURCE_DIR})
add_subdirectory(Common)
add_subdirectory(DataFormats)
add_subdirectory(Generator)
add_subdirectory(Geometry)
add_subdirectory(SimG4)

configure_file(cmake/lhecenv.in source-lhecenv.sh @ONLY)

install(EXPORT ${PROJECT_NAME}Targets
        NAMESPACE ${PROJECT_NAME}::
        FILE "${PROJECT_NAME}Targets.cmake"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}/")

#gaudi_install(CMAKE cmake/${PROJECT_NAME}Config.cmake)
