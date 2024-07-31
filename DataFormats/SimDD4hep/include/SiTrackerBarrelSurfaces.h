//==========================================================================
//  AIDA Detector description implementation
//--------------------------------------------------------------------------
// Copyright (C) Organisation europeenne pour la Recherche nucleaire (CERN)
// All rights reserved.
//
// For the licensing terms see $DD4hepINSTALL/LICENSE.
// For the list of contributors see $DD4hepINSTALL/doc/CREDITS.
//
// Author     : M.Frank
//
//==========================================================================
//
// Specialized generic detector constructor
//
//==========================================================================

#ifndef DataFormats_Simulation_SiTrackerBarrelSurfaces_h
#define DataFormats_Simulation_SiTrackerBarrelSurfaces_h

namespace {
  struct SiTrackerBarrelSurfaces {
    int dimension;
  };
}  // namespace

// Framework include files
#define SURFACEINSTALLER_DATA SiTrackerBarrelSurfaces
#define DD4HEP_USE_SURFACEINSTALL_HELPER Lhe_SiTrackerBarrelSurfacePlugin
#include <DD4hep/SurfaceInstaller.h>

#endif
