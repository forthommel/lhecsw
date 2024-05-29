#!/bin/bash

echo "Sourcing environment dirs for lxplus9"
export Geant4_DIR=/cvmfs/sft.cern.ch/lcg/releases/LCG_105b/Geant4/11.2.0/x86_64-el9-gcc13-opt/
export LCIO_DIR=/cvmfs/sft.cern.ch/lcg/releases/LCG_105b/LCIO/02.20/x86_64-el9-gcc13-opt/
export DD4hep_DIR=/cvmfs/sft.cern.ch/lcg/releases/LCG_105b/DD4hep/01.27.02/x86_64-el9-gcc13-opt/
source /cvmfs/beam-physics.cern.ch/bdsim/x86_64-el9-gcc13-opt/bdsim-env-v1.7.7-g4v10.7.2.3-ftfp-boost.sh
