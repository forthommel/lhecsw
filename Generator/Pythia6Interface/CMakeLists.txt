# look for Pythia 6
find_package(Pythia6)
if(NOT Pythia6_FOUND)
  return()
endif()

lhecsw_build(Generator_Pythia6Interface
    SOURCES src/*.cc
    LIBRARIES DD4hep::DDG4 ${Pythia6_LIBRARIES}
    HEADERS ${Pythia6_INCLUDE_DIRS})
