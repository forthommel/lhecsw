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

#include "Geometry/include/CaloBarrelPlaneImpl.h"

CaloBarrelPlaneImpl::CaloBarrelPlaneImpl(dd4hep::rec::SurfaceType typ,
                                         double thickness_inner,
                                         double thickness_outer,
                                         dd4hep::rec::Vector3D u_val,
                                         dd4hep::rec::Vector3D v_val,
                                         dd4hep::rec::Vector3D n_val,
                                         dd4hep::rec::Vector3D o_val,
                                         dd4hep::Volume vol,
                                         int id_val)
    : dd4hep::rec::VolPlaneImpl(typ, thickness_inner, thickness_outer, u_val, v_val, n_val, o_val, vol, id_val),
      _length(0),
      _width(0) {}

void CaloBarrelPlaneImpl::setData(double length, double width) {
  _length = length;
  _width = width;
}

void CaloBarrelPlaneImpl::setID(dd4hep::CellID id_val) { _id = id_val; }

bool CaloBarrelPlaneImpl::insideBounds(const dd4hep::rec::Vector3D& point, double epsilon) const {
  dd4hep::rec::Vector2D uvVec = globalToLocal(point);
  return std::abs(distance(point)) < epsilon && std::abs(uvVec[0]) < _width / 2. && std::abs(uvVec[1]) < _length / 2.;
}

std::vector<std::pair<dd4hep::rec::Vector3D, dd4hep::rec::Vector3D> > CaloBarrelPlaneImpl::getLines(unsigned) {
  return std::vector<std::pair<dd4hep::rec::Vector3D, dd4hep::rec::Vector3D> >{
      std::make_pair(origin() + _width / 2. * u() + _length / 2. * v(),
                     origin() - _width / 2. * u() + _length / 2. * v()),
      std::make_pair(origin() - _width / 2. * u() + _length / 2. * v(),
                     origin() - _width / 2. * u() - _length / 2. * v()),
      std::make_pair(origin() - _width / 2. * u() - _length / 2. * v(),
                     origin() + _width / 2. * u() - _length / 2. * v()),
      std::make_pair(origin() + _width / 2. * u() - _length / 2. * v(),
                     origin() + _width / 2. * u() + _length / 2. * v())};
}
