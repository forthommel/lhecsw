# look for CepGen
find_package(CepGen)
if(NOT CepGen_FOUND OR NOT HepMC3_FOUND)
  return()
endif()
find_package(nlohmann_json REQUIRED)

lhecsw_build(Generator_CepGenInterface
    SOURCES src/*.cc
    PLUGINS_SOURCES plugins/CepGen*.cc
    LIBRARIES Gaudi::GaudiKernel ${CepGen_LIBRARIES} ${HEPMC3_LIBRARIES} nlohmann_json::nlohmann_json
    HEADERS ${CepGen_INCLUDE_DIRS} ${HEPMC3_INCLUDE_DIR})
