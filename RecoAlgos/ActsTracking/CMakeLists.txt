find_package(Acts COMPONENTS Core PluginDD4hep)
if(NOT Acts_FOUND)
  return()
endif()

lhecsw_build(RecoAlgos_ActsTracking
    SOURCES src/*.cc
    PLUGINS_SOURCES plugins/*.cc
    LIBRARIES Gaudi::GaudiKernel
              ActsCore ActsFatras ActsPluginDD4hep
              k4FWCore::k4FWCore EDM4HEP::edm4hep)
