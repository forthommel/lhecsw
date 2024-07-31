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

#include "Geometry/DD4hep/include/SimpleCylinderImpl.h"

SimpleCylinderImpl::SimpleCylinderImpl(dd4hep::Volume vol,
                                       dd4hep::rec::SurfaceType type,
                                       double thickness_inner,
                                       double thickness_outer,
                                       dd4hep::rec::Vector3D origin)
    : dd4hep::rec::VolCylinderImpl(vol, type, thickness_inner, thickness_outer, origin), _half_length(0) {}

void SimpleCylinderImpl::setHalfLength(double half_length) { _half_length = half_length; }

void SimpleCylinderImpl::setID(dd4hep::rec::long64 id) { _id = id; }

bool SimpleCylinderImpl::insideBounds(const dd4hep::rec::Vector3D& point, double epsilon) const {
  return std::abs(point.rho() - origin().rho()) < epsilon && std::abs(point.z()) < _half_length;
}

std::vector<std::pair<dd4hep::rec::Vector3D, dd4hep::rec::Vector3D> > SimpleCylinderImpl::getLines(unsigned nMax) {
  std::vector<std::pair<dd4hep::rec::Vector3D, dd4hep::rec::Vector3D> > lines;

  lines.reserve(nMax);

  dd4hep::rec::Vector3D zv(0., 0., _half_length);
  double r = _o.rho();

  unsigned n = nMax / 4;
  double dPhi = 2. * ROOT::Math::Pi() / double(n);

  for (unsigned i = 0; i < n; ++i) {
    const dd4hep::rec::Vector3D rv0(r * sin(i * dPhi), r * cos(i * dPhi), 0.),
        rv1(r * sin((i + 1) * dPhi), r * cos((i + 1) * dPhi), 0.);
    const dd4hep::rec::Vector3D pl0 = zv + rv0, pl1 = zv + rv1, pl2 = -zv + rv1, pl3 = -zv + rv0;

    lines.push_back(std::make_pair(pl0, pl1));
    lines.push_back(std::make_pair(pl1, pl2));
    lines.push_back(std::make_pair(pl2, pl3));
    lines.push_back(std::make_pair(pl3, pl0));
  }
  return lines;
}
