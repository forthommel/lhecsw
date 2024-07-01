# look for CepGen
find_package(CepGen)
if(NOT CepGen_FOUND)
  return()
endif()

lhecsw_build(Generator_CepGenInterface
    SOURCES src/CepGen*.cc
    LIBRARIES DD4hep::DDG4 ${CepGen_LIBRARIES}
    HEADERS ${CepGen_INCLUDE_DIRS})
