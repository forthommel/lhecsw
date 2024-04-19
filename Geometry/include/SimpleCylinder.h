//==========================================================================
//  AIDA Detector description implementation
//--------------------------------------------------------------------------
// Copyright (C) Organisation europeenne pour la Recherche nucleaire (CERN)
// All rights reserved.
//
// For the licensing terms see $DD4hepINSTALL/LICENSE.
// For the list of contributors see $DD4hepINSTALL/doc/CREDITS.
//
//  Author     : M.Frank
//  Modified   : E.Pilicer ( tube + elliptical pipe layout )
//             : P.Kostka   ( more refined version circ-ell )
//
//====================================================================

#ifndef Geometry_SimpleCylinder_h
#define Geometry_SimpleCylinder_h

#include "Geometry/include/SimpleCylinderImpl.h"

class SimpleCylinder : public dd4hep::rec::VolSurface {
public:
  SimpleCylinder(dd4hep::Volume vol,
                 dd4hep::rec::SurfaceType type,
                 double thickness_inner,
                 double thickness_outer,
                 dd4hep::rec::Vector3D origin);
  SimpleCylinderImpl* operator->();
};

#endif
