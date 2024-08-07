# Locate Pythia6 library
# in a directory defined via Pythia6_DIR/PYTHIA6 environment variables
#
# Defines:
#  Pythia6_FOUND
#  Pythia6_LIBRARIES

set(Pythia6_DIRS $ENV{Pythia6_DIR} $ENV{PYTHIA6})

find_library(Pythia6_LIBRARY NAMES pythia6 Pythia6
             HINTS ${Pythia6_DIRS} PATH_SUFFIXES lib lib64 build/lib)

set(Pythia6_LIBRARIES ${Pythia6_LIBRARY})

# handle the QUIETLY and REQUIRED arguments and set Pythia6_FOUND to TRUE if
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Pythia6 DEFAULT_MSG Pythia6_LIBRARIES)

mark_as_advanced(Pythia6_FOUND Pythia6_LIBRARIES)
