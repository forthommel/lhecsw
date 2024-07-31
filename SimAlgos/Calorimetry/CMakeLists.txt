lhecsw_build(SimAlgos_Calorimetry
    PLUGINS_SOURCES plugins/*.cc
    LIBRARIES Gaudi::GaudiAlgLib k4FWCore::k4FWCore
              EDM4HEP::edm4hep
              DD4hep::DDCore)
