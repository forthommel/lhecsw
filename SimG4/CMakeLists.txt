file(GLOB sources src/*.cc)

gaudi_add_module(SimG4
                 SOURCES ${sources}
                 LINK k4FWCore::k4FWCore ${Geant4_LIBRARIES})
