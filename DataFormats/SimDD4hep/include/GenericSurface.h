//==========================================================================
//  Surface installer plugin for generic sliced detector drivers
//--------------------------------------------------------------------------
// Copyright (C) Organisation europeenne pour la Recherche nucleaire (CERN)
// All rights reserved.
//
// For the licensing terms see $DD4hepINSTALL/LICENSE.
// For the list of contributors see $DD4hepINSTALL/doc/CREDITS.
//
// Author     : N. Nikiforou, adapted from dd4hep/SiTrackerBarrel_surfaces.cpp
//              by M. Frank. Originally part of the lcgeo package
//==========================================================================

#ifndef DataFormats_Simulation_GenericSurface_h
#define DataFormats_Simulation_GenericSurface_h

namespace {
  struct GenericSurface {
    int dimension;
    double uvector[3];
    double vvector[3];
    double nvector[3];
    double ovector[3];
  };
}  // namespace

// Framework include files
#define SURFACEINSTALLER_DATA GenericSurface
#define DD4HEP_USE_SURFACEINSTALL_HELPER DD4hep_GenericSurfaceInstallerPlugin
#include <DD4hep/SurfaceInstaller.h>

#endif
