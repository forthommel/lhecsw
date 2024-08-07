# look for Pythia 6
find_package(Pythia6)
if(NOT Pythia6_FOUND)
  return()
endif()

lhecsw_build(Generator_Pythia6Interface
    SOURCES src/*.cc
    PLUGINS_SOURCES plugins/*.cc
    LIBRARIES k4FWCore::k4FWCore Gaudi::GaudiAlgLib ${Pythia6_LIBRARIES} ${HEPMC3_LIBRARIES}
    HEADERS ${Pythia6_INCLUDE_DIRS} ${HEPMC3_INCLUDE_DIRS} ${HEPMC3_INCLUDE_DIR})
