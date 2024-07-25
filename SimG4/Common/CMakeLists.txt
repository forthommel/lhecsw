lhecsw_build(SimG4_Common
    SOURCES src/*.cc
    PLUGINS_SOURCES plugins/*.cc
    LIBRARIES k4FWCore::k4FWCore DD4hep::DDCore EDM4HEP::edm4hep ${Geant4_LIBRARIES})
