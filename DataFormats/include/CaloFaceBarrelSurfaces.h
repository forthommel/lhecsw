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

#ifndef DataFormats_CaloFaceBarrelSurfaces_h
#define DataFormats_CaloFaceBarrelSurfaces_h

#include <string>

namespace {
  struct CaloFaceBarrelSurfaces {
    float length;
    float radius;
    float phi0;
    int symmetry;
    int systemID;
    std::string encoding;
  };
}  // namespace

// Framework include files
#define SURFACEINSTALLER_DATA CaloFaceBarrelSurfaces
#define DD4HEP_USE_SURFACEINSTALL_HELPER DD4hep_CaloFaceBarrelSurfacePlugin
#include <DD4hep/SurfaceInstaller.h>

#endif
