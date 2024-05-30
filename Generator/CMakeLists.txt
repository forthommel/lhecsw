find_package(Geant4)
find_package(Boost COMPONENTS system)
find_library(BDSIM bdsimRootEvent)

file(GLOB sources src/*.cc)
set(libraries DD4hep::DDG4)
set(includes)

if(BDSIM_FOUND AND Geant4_FOUND)  # link BDSIM/Geant4 includes directories
  list(APPEND libraries ${Boost_LIBRARIES})
  list(APPEND includes ${Geant4_INCLUDE_DIRS} ${BDSIM_INCLUDE_DIR})
else()  # remove all BDSIM-related references if not found on system
  file(GLOB bdsim_src src/BDSIMParticleGun.cc)
  list(REMOVE_ITEM sources ${bdsim_src})
endif()

lhecsw_build(Generator
    SOURCES ${sources}
    LIBRARIES ${libraries}
    HEADERS ${includes})
