##############################################################################
# cmake configuration file for DD4hep
#
# returns following variables:
#
#   DD4hep_FOUND      : set to TRUE if DD4hep found
#   DD4hep_VERSION    : package version
#   DD4hep_ROOT       : path to this DD4hep installation
#   DD4hep_LIBRARIES  : list of DD4hep libraries
#   DD4hep_INCLUDE_DIRS  : list of paths to be used with INCLUDE_DIRECTORIES
#   DD4hep_LIBRARY_DIRS  : list of paths to be used with LINK_DIRECTORIES
#
# @author Jan Engels, Desy
##############################################################################

# Locate the DD4hep install prefix. This CMake file is installed in two
# different locations (root and "cmake" subdir) that we must disambiguate.
get_filename_component(_thisdir "${CMAKE_CURRENT_LIST_FILE}" PATH)
get_filename_component(_lastdir "${_thisdir}" NAME)
if (_lastdir STREQUAL "cmake")
  get_filename_component(_thisdir "${_thisdir}" PATH)
endif()

set ( DD4hep_DIR                "${_thisdir}" )
set ( DD4hep_ROOT               "${_thisdir}" )
set ( DD4hep_VERSION            "1.25" )
set ( DD4hep_SOVERSION          "1.25" )
set ( DD4HEP_BUILD_DEBUG                       OFF )
set ( DD4HEP_USE_GEANT4                        OFF )
set ( DD4HEP_USE_CLHEP                          )
set ( DD4HEP_USE_LCIO                          OFF )
set ( DD4HEP_USE_EDM4HEP                       OFF )
set ( DD4HEP_USE_XERCESC                       OFF )
set ( DD4HEP_USE_TBB                           OFF )
set ( DD4HEP_USE_GEANT4_UNITS                  OFF )
set ( DD4HEP_RELAX_PYVER                       OFF )
set ( DD4hep_BUILD_CXX_STANDARD  14 )

include ( ${DD4hep_DIR}/cmake/DD4hep.cmake )

include(CMakeFindDependencyMacro)

set (CMAKE_MODULE_PATH  ${CMAKE_MODULE_PATH} ${DD4hep_DIR}/cmake  )

# ---------- include dirs -----------------------------------------------------
set(DD4hep_INCLUDE_DIRS "")


FIND_DEPENDENCY(Boost REQUIRED)

if(DD4HEP_USE_GEANT4)
  MESSAGE(STATUS "DD4hep uses Geant4")
  IF(DD4HEP_USE_CLHEP)
    find_dependency(CLHEP REQUIRED CONFIG)
    UNSET(CLHEP_INCLUDE_DIR)
  ENDIF()
  find_dependency(Geant4 10.2.2 REQUIRED gdml ui_all vis_all CONFIG)
  DD4HEP_SETUP_GEANT4_TARGETS()
endif()

FIND_DEPENDENCY(ROOT REQUIRED CONFIG)
DD4HEP_SETUP_ROOT_TARGETS()

if(DD4HEP_USE_LCIO)
  MESSAGE(STATUS "DD4hep uses LCIO")
  find_dependency(LCIO REQUIRED CONFIG)
  DD4HEP_SETUP_LCIO_TARGETS()
endif()

if(DD4HEP_USE_TBB)
  MESSAGE(STATUS "DD4hep uses TBB")
  find_dependency(TBB REQUIRED CONFIG)
endif()

if(NOT CMAKE_PROJECT_NAME STREQUAL DD4hep)
  include("${DD4hep_ROOT}/cmake/DD4hepConfig-targets.cmake")
endif()

# Backward compatible DD4hep_INCLUDE_DIRS and DD4hep_LIBRARIES variables
GET_TARGET_PROPERTY(DD4hep_INCLUDE_DIRS DD4hep::DDCore INTERFACE_INCLUDE_DIRECTORIES)
set(DD4hep_LIBRARIES DD4hep::DDCore)

# Backward compatible DD4hep_COMPONENT_LIBRARIES variable
SET(DD4hep_COMPONENT_LIBRARIES)
IF(DD4hep_FIND_COMPONENTS)
  dd4hep_print( "|++> DD4hep: Looking for Components: ${DD4hep_FIND_COMPONENTS}" )
  FOREACH(comp ${DD4hep_FIND_COMPONENTS})
    IF(NOT TARGET DD4hep::${comp})
      MESSAGE(FATAL_ERROR "|++> Did not find required component: ${comp}")
    ENDIF()
    LIST(APPEND DD4hep_COMPONENT_LIBRARIES DD4hep::${comp})
  ENDFOREACH()
ENDIF()

#---- build with xercesc or tinyxml ?
INCLUDE( ${DD4hep_DIR}/cmake/DD4hep_XML_setup.cmake )

# ---------- final checking ---------------------------------------------------
INCLUDE( FindPackageHandleStandardArgs )
# set DD4HEP_FOUND to TRUE if all listed variables are TRUE and not empty
FIND_PACKAGE_HANDLE_STANDARD_ARGS( DD4hep DEFAULT_MSG DD4hep_DIR DD4hep_INCLUDE_DIRS DD4hep_LIBRARIES )

SET( DD4hep_FOUND ${DD4hep_FOUND} )
