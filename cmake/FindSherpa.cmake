find_package(PkgConfig)
pkg_check_modules(PC_Sherpa QUIET Sherpa)

set(SHERPA_DIRS $ENV{SHERPA_ROOT_DIR} $ENV{SHERPA_DIR} ${SHERPA_ROOT_DIR})

# Defines:
#  Sherpa_FOUND
#  Sherpa_INCLUDE_DIR
#  Sherpa_INCLUDE_DIRS (not cached)
#  Sherpa_LIBRARY
#  Sherpa_LIBRARIES (not cached)
#  Sherpa_LIBRARY_DIRS (not cached)

find_path(Sherpa_INCLUDE_DIR
    NAMES SHERPA/Main/Sherpa.H
    HINTS ${SHERPA_DIRS}/include
    PATH_SUFFIXES include SHERPA-MC)
find_library(Sherpa_LIBRARY
    NAMES SherpaMain
    HINTS ${SHERPA_DIRS}
    PATH_SUFFIXES lib/SHERPA-MC lib64/SHERPA-MC)

mark_as_advanced(Sherpa_INCLUDE_DIR)

# Enforce a minimal list if none is explicitly requested
if(NOT Sherpa_FIND_COMPONENTS)
    set(Sherpa_FIND_COMPONENTS
        SherpaMain
        ModelMain
        METoolsMain
        METoolsLoops
        METoolsSpinCorrelations
        PDF
        PhotonsMEs
        PhotonsMain
        PhotonsPhaseSpace
        PhotonsTools
        Remnant
        ToolsMath
        ToolsOrg
        ToolsPhys)
endif()

foreach(component ${Sherpa_FIND_COMPONENTS})
    find_library(Sherpa_${component}_LIBRARY
        NAMES ${component} ${_${component}_names}
        HINTS ${SHERPA_DIRS} ${SHERPA_DIRS}/lib64
        PATH_SUFFIXES SHERPA-MC)
    if (Sherpa_${component}_LIBRARY)
        set(Sherpa_${component}_FOUND 1)
        list(APPEND Sherpa_LIBRARIES ${Sherpa_${component}_LIBRARY})
        get_filename_component(libdir ${Sherpa_${component}_LIBRARY} PATH)
        list(APPEND Sherpa_LIBRARY_DIRS ${libdir})
    else()
        set(Sherpa_${component}_FOUND 0)
    endif()
    mark_as_advanced(Sherpa_${component}_LIBRARY)
endforeach()
if(Sherpa_LIBRARY_DIRS)
  list(REMOVE_DUPLICATES Sherpa_LIBRARY_DIRS)
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Sherpa
    FOUND_VAR Sherpa_FOUND
    REQUIRED_VARS Sherpa_LIBRARY Sherpa_INCLUDE_DIR)

if(Sherpa_FOUND)
    set(Sherpa_LIBRARIES ${Sherpa_LIBRARY})
    set(Sherpa_INCLUDE_DIRS ${Sherpa_INCLUDE_DIR})
    set(Sherpa_DEFINITIONS ${PC_Sherpa_CFLAGS_OTHER})
endif()

mark_as_advanced(Sherpa_INCLUDE_DIR Sherpa_LIBRARY)
