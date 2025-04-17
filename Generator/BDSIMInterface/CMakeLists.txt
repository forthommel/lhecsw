# look for BDSIM library
set(BDSIM_DIRS $ENV{BDSIM_DIR} ${BDSIM_DIR} /usr /usr/local)
find_library(BDSIM_LIBRARY bdsim
             HINTS ${BDSIM_DIRS}
             PATH_SUFFIXES lib lib64)
if(NOT BDSIM_LIBRARY OR NOT HepMC3_FOUND)
  return()
endif()
# look for BDSIM headers
find_path(BDSIM_INCLUDE_DIR BDSOutputROOTEventSampler.hh
          HINTS ${BDSIM_DIRS}
          PATH_SUFFIXES include include/bdsim)
if(NOT BDSIM_INCLUDE_DIR)
  return()
endif()

lhecsw_build(Generator_BDSIMInterface
    PLUGINS_SOURCES plugins/BDSIMParticleGun.cc
    LIBRARIES Gaudi::GaudiKernel ${Boost_LIBRARIES} ${BDSIM_LIBRARY} ${HEPMC3_LIBRARY}
    HEADERS ${Geant4_INCLUDE_DIRS} ${BDSIM_INCLUDE_DIR} ${HEPMC3_INCLUDE_DIR})
