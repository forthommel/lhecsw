#[[lhecsw_build(Geometry
    SOURCES src/*.cc
    PLUGINS_SOURCES plugins/*.cc
    LIBRARIES DD4hep::DDAlign DD4hep::DDCond DD4hep::DDCore DD4hep::DDG4 DD4hep::DDParsers DD4hep::DDRec
              ROOT::Core ROOT::GenVector ROOT::Geom ROOT::MathCore)
#]]

file(GLOB sources src/*.cc)
file(GLOB plugins_sources plugins/*.cc)
add_library(Geometry_DD4hep ${sources} ${plugins_sources})
target_link_libraries(Geometry_DD4hep DD4hep::DDAlign DD4hep::DDCond DD4hep::DDCore DD4hep::DDG4 DD4hep::DDParsers DD4hep::DDRec
       ROOT::Core ROOT::GenVector ROOT::Geom ROOT::MathCore)
dd4hep_generate_rootmap(Geometry_DD4hep)
