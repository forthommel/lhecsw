#!/bin/bash

echo "Sourcing environment dirs for lxplus9 [zsh|bash]"

export PATH=/cvmfs/sft.cern.ch/lcg/releases/LCG_105b/CMake/3.26.2/x86_64-el9-gcc13-opt/bin:$PATH
source /cvmfs/sft.cern.ch/lcg/contrib/gcc/13/x86_64-el9/setup.sh

export DD4hep_DIR=/cvmfs/sft.cern.ch/lcg/releases/LCG_105b/DD4hep/01.27.02/x86_64-el9-gcc13-opt/

export Python_ROOT_DIR=/cvmfs/sft.cern.ch/lcg/releases/LCG_105b/Python/3.9.12/x86_64-el9-gcc13-opt/
export Boost_DIR=/cvmfs/sft.cern.ch/lcg/releases/LCG_105b/Boost/1.82.0/x86_64-el9-gcc13-opt/
export LCIO_DIR=/cvmfs/sft.cern.ch/lcg/releases/LCG_105b/LCIO/02.20/x86_64-el9-gcc13-opt/
export Qt5_DIR=/cvmfs/sft.cern.ch/lcg/releases/LCG_105b/qt5/5.15.9/x86_64-el9-gcc13-opt/
export TBB_DIR=/cvmfs/sft.cern.ch/lcg/releases/LCG_105b/tbb/2021.10.0/x86_64-el9-gcc13-opt/
export VDT_DIR=/cvmfs/sft.cern.ch/lcg/releases/LCG_105b/vdt/0.4.4/x86_64-el9-gcc13-opt/
export Vc_DIR=/cvmfs/sft.cern.ch/lcg/releases/LCG_104c/Vc/1.4.4/x86_64-el9-gcc13-opt/

export CMAKE_PREFIX_PATH=$Qt5_DIR:$VDT_DIR:$CMAKE_PREFIX_PATH

export BDSIM_DIR=/cvmfs/beam-physics.cern.ch/bdsim/x86_64-el9-gcc13-opt/
export Geant4_DIR=$BDSIM_DIR/geant4-v10.7.2.3-ftfp/
export CLHEP_DIR=/cvmfs/sft.cern.ch/lcg/releases/LCG_104c/clhep/2.4.6.4/x86_64-el9-gcc13-opt/

source /cvmfs/sft.cern.ch/lcg/releases/LCG_105b/ROOT/6.30.06/x86_64-el9-gcc13-opt/bin/thisroot.sh
export ROOT_INCLUDE_PATH=$DD4hep_DIR/include:$ROOTSYS/include:$Vc_DIR/include:$TBB_DIR/include:$BDSIM_DIR/include:$ROOT_INCLUDE_PATH
export LD_LIBRARY_PATH=$DD4hep_DIR/lib:$TBB_DIR/lib64:$Qt5_DIR/lib:$LD_LIBRARY_PATH
