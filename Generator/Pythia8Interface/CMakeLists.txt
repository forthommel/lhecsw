# look for Pythia 8
find_package(Pythia8)
if(NOT Pythia8_FOUND OR NOT HepMC3_FOUND)
  return()
endif()

lhecsw_build(Generator_Pythia8Interface
    SOURCES src/Pythia8*.cc
    LIBRARIES DD4hep::DDG4 ${Pythia8_LIBRARIES} ${HEPMC3_LIBRARIES}
    HEADERS ${Pythia8_INCLUDE_DIRS} ${HEPMC3_INCLUDE_DIRS} ${HEPMC3_INCLUDE_DIR})
