//==========================================================================
//  AIDA Detector description implementation
//--------------------------------------------------------------------------
// Copyright (C) Organisation europeenne pour la Recherche nucleaire (CERN)
// All rights reserved.
//
// For the licensing terms see $DD4hepINSTALL/LICENSE.
// For the list of contributors see $DD4hepINSTALL/doc/CREDITS.
//
// Author     : M.Frank
//
//==========================================================================
//
// Specialized generic detector constructor
//
//==========================================================================

#include "DataFormats/Simulation/include/SiTrackerEndcapSurfaces.h"

/// Install measurement surfaces
template <typename T>
void Installer<T>::install(dd4hep::DetElement component, dd4hep::PlacedVolume pv) {
  dd4hep::Volume comp_vol = pv.volume();
  if (!comp_vol.isSensitive())
    return;

  const dd4hep::Trapezoid comp_shape(comp_vol.solid()), mod_shape(parentVolume(component).solid());
  if (!comp_shape.isValid() || !mod_shape.isValid())
    invalidInstaller("Components and/or modules are not Trapezoid -- invalid shapes");
  else if (!handleUsingCache(component, comp_vol)) {
    const double* trans = placementTranslation(component);
    const auto half_mod_thickness = mod_shape->GetDy1(), si_position = trans[1];
    const auto outer_thickness = half_mod_thickness + si_position, inner_thickness = half_mod_thickness - si_position;
    const Vector3D u(0., 0., -1.), v(-1., 0., 0.), n(0., -1., 0.), o(0., 0., 0.);

    VolPlane surf(comp_vol, Type(Type::Sensitive, Type::Measurement1D), inner_thickness, outer_thickness, u, v, n, o);
    addSurface(component, surf);
  }
}
