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

#ifndef Geometry_SimpleCylinderImpl_h
#define Geometry_SimpleCylinderImpl_h

#include <DDRec/Surface.h>

#include <map>
#include <vector>

/// helper class for a simple cylinder surface parallel to z with a given length - used as IP layer
class SimpleCylinderImpl : public dd4hep::rec::VolCylinderImpl {
public:
  /// standard c'tor with all necessary arguments - origin is (0,0,0) if not given.
  explicit SimpleCylinderImpl(dd4hep::Volume vol,
                              dd4hep::rec::SurfaceType type,
                              double thickness_inner,
                              double thickness_outer,
                              dd4hep::rec::Vector3D origin);

  void setHalfLength(double half_length);
  void setID(dd4hep::rec::long64 id);
  // overwrite to include points inside the inner radius of the barrel
  bool insideBounds(const dd4hep::rec::Vector3D& point, double epsilon) const;

  virtual std::vector<std::pair<dd4hep::rec::Vector3D, dd4hep::rec::Vector3D> > getLines(unsigned nMax = 100);

private:
  double _half_length;
};

#endif
