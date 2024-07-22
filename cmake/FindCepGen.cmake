find_package(PkgConfig)
pkg_check_modules(PC_CepGen QUIET CepGen)

set(CepGen_DIRS $ENV{CEPGEN_PATH} ${PC_CepGen_INCLUDE_DIRS} ${CEPGEN_PATH} ${Cepgen_DIR})
find_path(CepGen_INCLUDE_DIR
    NAMES CepGen/Generator.h
    PATHS ${CepGen_DIRS}
    PATH_SUFFIXES include)
find_path(CepGen_DATA_DIR
    NAMES mstw_sf_scan_nnlo.dat
    PATHS ${CepGen_DIRS}
    PATH_SUFFIXES share/CepGen External)
find_library(CepGen_LIBRARY
    NAMES CepGen
    PATHS ${CepGen_DIRS}
    PATH_SUFFIXES lib64 lib build)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(CepGen
    FOUND_VAR CepGen_FOUND
    REQUIRED_VARS CepGen_LIBRARY CepGen_INCLUDE_DIR)

if(CepGen_FOUND)
    set(CepGen_LIBRARIES ${CepGen_LIBRARY})
    set(CepGen_INCLUDE_DIRS ${CepGen_INCLUDE_DIR})
    set(CepGen_DATA_DIRS ${CepGen_DATA_DIR})
    set(CepGen_DEFINITIONS ${PC_CepGen_CFLAGS_OTHER})
else()
    message(STATUS "CepGen path was not found. Did you set the $CEPGEN_PATH environment variable?")
endif()

if(CepGen_FOUND AND NOT TARGET CepGen::CepGen)
  add_library(CepGen::CepGen UNKNOWN IMPORTED)
  set_target_properties(CepGen::CepGen PROPERTIES
    IMPORTED_LOCATION "${CepGen_LIBRARY}"
    INTERFACE_COMPILE_OPTIONS "${PC_CepGen_CFLAGS_OTHER}"
    INTERFACE_INCLUDE_DIRECTORIES "${CepGen_INCLUDE_DIR}")
endif()

mark_as_advanced(CepGen_INCLUDE_DIR CepGen_LIBRARY)
