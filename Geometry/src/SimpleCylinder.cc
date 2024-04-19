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

#include "Geometry/include/SimpleCylinder.h"

SimpleCylinder::SimpleCylinder(dd4hep::Volume vol,
                               dd4hep::rec::SurfaceType type,
                               double thickness_inner,
                               double thickness_outer,
                               dd4hep::rec::Vector3D origin)
    : dd4hep::rec::VolSurface(new SimpleCylinderImpl(vol, type, thickness_inner, thickness_outer, origin)) {}

SimpleCylinderImpl* SimpleCylinder::operator->() { return static_cast<SimpleCylinderImpl*>(_surf); }
