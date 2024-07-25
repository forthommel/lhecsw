lhecsw_build(SimG4_Tracker
    PLUGINS_SOURCES plugins/*.cc
    LIBRARIES k4FWCore::k4FWCore DD4hep::DDCore DD4hep::DDG4 EDM4HEP::edm4hep ${Geant4_LIBRARIES})
