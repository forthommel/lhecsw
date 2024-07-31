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

#ifndef Geometry_DD4hep_CaloBarrelPlaneImpl_h
#define Geometry_DD4hep_CaloBarrelPlaneImpl_h

#include <DDRec/Surface.h>

/// helper class for a planar surface placed into a polyhedral calorimeter barrel
class CaloBarrelPlaneImpl : public dd4hep::rec::VolPlaneImpl {
public:
  /// standard c'tor with all necessary arguments - origin is (0,0,0) if not given.
  explicit CaloBarrelPlaneImpl(dd4hep::rec::SurfaceType typ,
                               double thickness_inner,
                               double thickness_outer,
                               dd4hep::rec::Vector3D u_val,
                               dd4hep::rec::Vector3D v_val,
                               dd4hep::rec::Vector3D n_val,
                               dd4hep::rec::Vector3D o_val,
                               dd4hep::Volume vol,
                               int id_val);

  void setData(double length, double width);
  void setID(dd4hep::CellID id_val);

  // overwrite to include points inside the inner radius of the barrel
  bool insideBounds(const dd4hep::rec::Vector3D& point, double epsilon) const;

  /// create outer bounding lines for the given symmetry of the polyhedron
  virtual std::vector<std::pair<dd4hep::rec::Vector3D, dd4hep::rec::Vector3D> > getLines(unsigned);

private:
  double _length, _width;
};

#endif
