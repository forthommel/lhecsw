set(BDSIM_DIRS $ENV{BDSIM_DIR} ${BDSIM_DIR} /usr /usr/local)
find_library(BDSIM_LIBRARY bdsim
             HINTS ${BDSIM_DIRS}
             PATH_SUFFIXES lib lib64)
find_path(BDSIM_INCLUDE_DIR BDSOutputROOTEventSampler.hh
          HINTS ${BDSIM_DIRS}
          PATH_SUFFIXES include include/bdsim)

file(GLOB sources src/*.cc)
set(libraries DD4hep::DDG4)
set(includes)

if(BDSIM_LIBRARY AND BDSIM_INCLUDE_DIR)  # link BDSIM includes directory
  list(APPEND libraries ${Boost_LIBRARIES} ${BDSIM_LIBRARY})
  list(APPEND includes ${Geant4_INCLUDE_DIRS} ${BDSIM_INCLUDE_DIR})
else()  # remove all BDSIM-related references if not found on system
  file(GLOB bdsim_src src/BDSIMParticleGun.cc)
  list(REMOVE_ITEM sources ${bdsim_src})
endif()

lhecsw_build(Generator
    SOURCES ${sources}
    LIBRARIES ${libraries}
    HEADERS ${includes})
