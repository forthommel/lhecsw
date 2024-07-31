#!/bin/bash

echo "Sourcing environment dirs for lxplus9 [zsh|bash]"

source /cvmfs/sw.hsf.org/key4hep/setup.sh

export PATH=/cvmfs/sft.cern.ch/lcg/releases/LCG_104/CMake/3.26.2/x86_64-el9-gcc11-opt/bin:$PATH
export PATH=/cvmfs/sft.cern.ch/lcg/releases/LCG_104/ninja/1.10.0/x86_64-el9-gcc13-opt/bin:$PATH

source /cvmfs/sft.cern.ch/lcg/releases/gcc/11.3.1/x86_64-centos9/setup.sh

export lhecsw=`git rev-parse --show-toplevel`
