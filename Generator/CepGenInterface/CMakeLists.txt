# look for CepGen
find_package(CepGen)
if(NOT CepGen_FOUND OR NOT HepMC3_FOUND)
  return()
endif()

lhecsw_build(Generator_CepGenInterface
    PLUGINS_SOURCES plugins/CepGen*.cc
    LIBRARIES Gaudi::GaudiAlgLib ${CepGen_LIBRARIES} ${HEPMC3_LIBRARIES}
    HEADERS ${CepGen_INCLUDE_DIRS} ${HEPMC3_INCLUDE_DIR})
