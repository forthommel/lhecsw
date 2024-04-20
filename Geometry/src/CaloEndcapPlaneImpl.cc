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

#include "Geometry/include/CaloEndcapPlaneImpl.h"

CaloEndcapPlaneImpl::CaloEndcapPlaneImpl(dd4hep::rec::SurfaceType typ,
                                         double thickness_inner,
                                         double thickness_outer,
                                         dd4hep::rec::Vector3D u_val,
                                         dd4hep::rec::Vector3D v_val,
                                         dd4hep::rec::Vector3D n_val,
                                         dd4hep::rec::Vector3D o_val,
                                         dd4hep::Volume vol,
                                         int id_val)
    : dd4hep::rec::VolPlaneImpl(typ, thickness_inner, thickness_outer, u_val, v_val, n_val, o_val, vol, id_val),
      _r(0),
      _phi0(0),
      _sym(0) {}

void CaloEndcapPlaneImpl::setData(double radius, double phi0, unsigned symmetry) {
  _r = radius;
  _phi0 = phi0;
  _sym = symmetry;
}

void CaloEndcapPlaneImpl::setID(dd4hep::CellID id_val) { _id = id_val; }

bool CaloEndcapPlaneImpl::insideBounds(const dd4hep::rec::Vector3D& point, double epsilon) const {
  double ri = _r * cos(2. * M_PI / _sym);

  return std::abs(distance(point)) < epsilon &&
         (point.rho() < ri || volume()->GetShape()->Contains(const_cast<double*>(point.const_array())));
}

std::vector<std::pair<dd4hep::rec::Vector3D, dd4hep::rec::Vector3D> > CaloEndcapPlaneImpl::getLines(unsigned) {
  std::vector<std::pair<dd4hep::rec::Vector3D, dd4hep::rec::Vector3D> > lines;

  double alpha = (_sym ? 2. * M_PI / _sym : 0.);

  for (unsigned i = 0; i < _sym; ++i) {
    double gam0 = i * alpha + _phi0;
    double gam1 = (i + 1) * alpha + _phi0;
    lines.push_back(std::make_pair(dd4hep::rec::Vector3D(_r * cos(gam0), _r * sin(gam0), origin().z()),
                                   dd4hep::rec::Vector3D(_r * cos(gam1), _r * sin(gam1), origin().z())));
  }
  return lines;
}
