set(sources)
set(libraries DD4hep::DDG4)
set(headers)

# look for HepMC 3
find_package(HepMC3)
if(HepMC3_FOUND)
  list(APPEND sources src/HepMC3EventConverter.cc)
  list(APPEND libraries ${HEPMC3_LIBRARIES})
  list(APPEND headers ${HEPMC3_INCLUDE_DIRS} ${HEPMC3_INCLUDE_DIR})
endif()

lhecsw_build(Generator_Common
    SOURCES ${sources}
    LIBRARIES ${libraries}
    HEADERS ${headers})
