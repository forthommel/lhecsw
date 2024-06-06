set(sources)
set(libraries DD4hep::DDG4)
set(includes)

# NOTE: this set of library-specific add-ons deserves a splitting into a deeper
# hierarchy of the Generator package.

# look for BDSIM
set(BDSIM_DIRS $ENV{BDSIM_DIR} ${BDSIM_DIR} /usr /usr/local)
find_library(BDSIM_LIBRARY bdsim
             HINTS ${BDSIM_DIRS}
             PATH_SUFFIXES lib lib64)
find_path(BDSIM_INCLUDE_DIR BDSOutputROOTEventSampler.hh
          HINTS ${BDSIM_DIRS}
          PATH_SUFFIXES include include/bdsim)


if(BDSIM_LIBRARY AND BDSIM_INCLUDE_DIR)  # link BDSIM includes directory
  file(GLOB bdsim_src src/BDSIMParticleGun.cc)
  list(APPEND sources ${bdsim_src})
  list(APPEND libraries ${Boost_LIBRARIES} ${BDSIM_LIBRARY})
  list(APPEND includes ${Geant4_INCLUDE_DIRS} ${BDSIM_INCLUDE_DIR})
endif()

lhecsw_build(Generator
    SOURCES ${sources}
    LIBRARIES ${libraries}
    HEADERS ${includes})
