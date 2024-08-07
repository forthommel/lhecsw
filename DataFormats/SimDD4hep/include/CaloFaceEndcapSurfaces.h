//==========================================================================
//  AIDA Detector description implementation
//--------------------------------------------------------------------------
// Copyright (C) Organisation europeenne pour la Recherche nucleaire (CERN)
// All rights reserved.
//
// For the licensing terms see $DD4hepINSTALL/LICENSE.
// For the list of contributors see $DD4hepINSTALL/doc/CREDITS.
//
// Author     : F.Gaede DESY
//
//==========================================================================

#ifndef DataFormats_Simulation_CaloFaceEndcapSurfaces_h
#define DataFormats_Simulation_CaloFaceEndcapSurfaces_h

#include <string>

namespace {
  struct CaloFaceEndcapSurfaces {
    float zpos;
    float radius;
    float phi0;
    int symmetry;
    int systemID;
    std::string encoding;
  };
}  // namespace

// Framework include files
#define SURFACEINSTALLER_DATA CaloFaceEndcapSurfaces
#define DD4HEP_USE_SURFACEINSTALL_HELPER DD4hep_CaloFaceEndcapSurfacePlugin
#include <DD4hep/SurfaceInstaller.h>

#endif
