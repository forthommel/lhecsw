# Locate Pythia8 library
# in a directory defined via Pythia8_DIR/PYTHIA8 environment variables
#
# Defines:
#  Pythia8_FOUND
#  Pythia8_LIBRARIES
#  Pythia8_INCLUDES

set(Pythia8_DIRS $ENV{Pythia8_DIR} $ENV{PYTHIA8})

find_library(Pythia8_LIBRARY NAMES pythia8
             HINTS ${Pythia8_DIRS} PATH_SUFFIXES lib lib64 build/lib)

set(Pythia8_LIBRARIES ${Pythia8_LIBRARY})

find_path(Pythia8_INCLUDE_DIR Pythia8/Pythia.h
          HINTS ${Pythia8_DIRS} PATH_SUFFIXES include)
set(Pythia8_INCLUDE_DIRS ${Pythia8_INCLUDE_DIR})

# handle the QUIETLY and REQUIRED arguments and set Pythia8_FOUND to TRUE if
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Pythia8 DEFAULT_MSG Pythia8_LIBRARIES Pythia8_INCLUDE_DIRS)

mark_as_advanced(Pythia8_FOUND Pythia8_LIBRARIES Pythia8_INCLUDE_DIRS)
