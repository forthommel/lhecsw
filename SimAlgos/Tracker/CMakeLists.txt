lhecsw_build(SimAlgos_Tracker
    PLUGINS_SOURCES plugins/*.cc
    LIBRARIES Gaudi::GaudiKernel k4FWCore::k4FWCore
              EDM4HEP::edm4hep
              DD4hep::DDCore)
