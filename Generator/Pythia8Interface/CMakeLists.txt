# look for Pythia 8
find_package(Pythia8)
if(NOT Pythia8_FOUND OR NOT HepMC3_FOUND)
  return()
endif()

lhecsw_build(Generator_Pythia8Interface
    PLUGINS_SOURCES plugins/Pythia8*.cc
    LIBRARIES k4FWCore::k4FWCore Gaudi::GaudiKernel ${Pythia8_LIBRARIES} ${HEPMC3_LIBRARIES}
    HEADERS ${Pythia8_INCLUDE_DIRS} ${HEPMC3_INCLUDE_DIRS} ${HEPMC3_INCLUDE_DIR})
