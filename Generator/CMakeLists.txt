find_package(BDSIM)
find_package(Geant4)

set(includes)
file(GLOB sources src/*.cc)
if(NOT BDSIM_FOUND OR NOT Geant4_FOUND)
  file(GLOB bdsim_src src/BDSIMParticleGun.cc)
  list(REMOVE_ITEM sources ${bdsim_src})
else()
  list(APPEND includes ${BDSIM_INCLUDE_DIR} ${Geant4_INCLUDE_DIRS})
endif()

lhecsw_build(Generator
    SOURCES ${sources}
    HEADERS ${includes})
