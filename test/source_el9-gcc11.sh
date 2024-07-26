#!/bin/bash

echo "Sourcing environment dirs for lxplus9 [zsh|bash]"

export PATH=/cvmfs/sft.cern.ch/lcg/releases/LCG_105b/CMake/3.26.2/x86_64-el9-gcc13-opt/bin:$PATH
export PATH=/cvmfs/sft.cern.ch/lcg/releases/LCG_105b/ninja/1.10.0/x86_64-el9-gcc13-opt/bin:$PATH

source /cvmfs/sft.cern.ch/lcg/views/LCG_105b/x86_64-el9-gcc13-opt/setup.sh
source /cvmfs/sft.cern.ch/lcg/releases/LCG_105b/DD4hep/01.27.02/x86_64-el9-gcc13-opt/bin/thisdd4hep.sh

export Python_ROOT_DIR=/cvmfs/sft.cern.ch/lcg/releases/LCG_105b/Python/3.9.12/x86_64-el9-gcc13-opt/
export Boost_DIR=/cvmfs/sft.cern.ch/lcg/releases/LCG_105b/Boost/1.82.0/x86_64-el9-gcc13-opt/
export LCIO_DIR=/cvmfs/sft.cern.ch/lcg/releases/LCG_105b/LCIO/02.20/x86_64-el9-gcc13-opt/
export Qt5_DIR=/cvmfs/sft.cern.ch/lcg/releases/LCG_105b/qt5/5.15.9/x86_64-el9-gcc13-opt/
export TBB_DIR=/cvmfs/sft.cern.ch/lcg/releases/LCG_105b/tbb/2021.10.0/x86_64-el9-gcc13-opt/
export VDT_DIR=/cvmfs/sft.cern.ch/lcg/releases/LCG_105b/vdt/0.4.4/x86_64-el9-gcc13-opt/
export Vc_DIR=/cvmfs/sft.cern.ch/lcg/releases/LCG_105b/Vc/1.4.4/x86_64-el9-gcc13-opt/

export CMAKE_PREFIX_PATH=$Qt5_DIR:$VDT_DIR:$CMAKE_PREFIX_PATH

export BDSIM_DIR=/eos/user/l/lforthom/public/bdsim-v1.7.7-rc2/
export Geant4_DIR=/cvmfs/sft.cern.ch/lcg/releases/LCG_105b/Geant4/11.2.0/x86_64-el9-gcc13-opt/
export CLHEP_DIR=/cvmfs/sft.cern.ch/lcg/releases/LCG_105b/clhep/2.4.7.1/x86_64-el9-gcc13-opt/

source /cvmfs/sft.cern.ch/lcg/releases/LCG_105b/ROOT/6.30.06/x86_64-el9-gcc13-opt/bin/thisroot.sh
export ROOT_INCLUDE_PATH=$DD4hep_DIR/include:$ROOTSYS/include:$CLHEP_DIR/include:$Vc_DIR/include:$TBB_DIR/include:$BDSIM_DIR/include:$ROOT_INCLUDE_PATH
export LD_LIBRARY_PATH=$DD4hep_DIR/lib:$TBB_DIR/lib64:$Vc_DIR/lib:$Qt5_DIR/lib:$LD_LIBRARY_PATH

export lhecsw=`git rev-parse --show-toplevel`
