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

#include <DD4hep/Printout.h>

#include "DataFormats/include/PolyhedraEndcapCalorimeter2Surfaces.h"

/// Install measurement surfaces
template <typename UserData>
void Installer<UserData>::install(dd4hep::DetElement component, dd4hep::PlacedVolume pv) {
  dd4hep::Volume comp_vol = pv.volume();
  if (!comp_vol.isSensitive())
    return;
  dd4hep::PolyhedraRegular comp_shape(comp_vol.solid()), mod_shape(parentVolume(component).solid());
  if (!comp_shape.isValid() || !mod_shape.isValid())
    invalidInstaller("Components and/or modules are not Trapezoid -- invalid shapes");
  else if (!handleUsingCache(component, comp_vol)) {
    const auto m = component.parent().nominal().worldTransformation();
    const double* trans = placementTranslation(component);
    const auto half_mod_thickness = 0.5 * (mod_shape->GetZ(1) - mod_shape->GetZ(0)),
               half_comp_thickness = 0.5 * (comp_shape->GetZ(1) - comp_shape->GetZ(0));
    const auto si_position = trans[2] + half_mod_thickness, outer_thickness = half_mod_thickness - si_position,
               inner_thickness = half_mod_thickness + si_position;
    dd4hep::printout(dd4hep::DEBUG,
                     "PolyhedraEndcapCalorimeter2Surfaces::install",
                     " Module:    %s\n"
                     " Component: %s\ndz: %g Si-pos: %g Mod-thickness: %g Comp-thickness: %g",
                     mod_shape.toString(),
                     comp_shape.toString(),
                     m.GetTranslation()[2],
                     si_position,
                     half_mod_thickness,
                     half_comp_thickness);

    const Vector3D u(1., 0., 0.), v(0., 1., 0.), n(0., 0., 1.), o(100., 100., 0.);

    VolPlane surf(comp_vol, Type(Type::Sensitive, Type::Measurement1D), inner_thickness, outer_thickness, u, v, n, o);
    addSurface(component, surf);
  }
}
