# look for Sherpa
find_package(Sherpa)
if(NOT Sherpa_FOUND OR NOT HepMC3_FOUND)
  return()
endif()

lhecsw_build(Generator_SherpaInterface
    PLUGINS_SOURCES plugins/*.cc
    LIBRARIES k4FWCore::k4FWCore Gaudi::GaudiAlgLib ${Shepa_LIBRARIES} ${HEPMC3_LIBRARIES}
    HEADERS ${Sherpa_INCLUDE_DIRS} ${HEPMC3_INCLUDE_DIRS} ${HEPMC3_INCLUDE_DIR})
