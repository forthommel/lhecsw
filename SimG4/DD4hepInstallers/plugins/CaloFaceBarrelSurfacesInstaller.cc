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

#include <DD4hep/Printout.h>
#include <DDRec/DetectorData.h>
#include <DDRec/Surface.h>
#include <DDSegmentation/BitField64.h>

#include "DataFormats/SimDD4hep/include/CaloFaceBarrelSurfaces.h"
#include "Geometry/include/CaloBarrelPlane.h"

template <>
void Installer<CaloFaceBarrelSurfaces>::handle_arguments(int argc, char** argv) {
  for (int i = 0; i < argc; ++i) {
    if (char* ptr = ::strchr(argv[i], '='); ptr) {
      std::string name(argv[i], ptr);
      const auto value = dd4hep::_toDouble(++ptr);

      printout(dd4hep::DEBUG, "DD4hep_CaloFaceBarrelSurfacePlugin", "argument[%d] = %s = %f", i, name.c_str(), value);

      if (name == "length")
        data.length = value;
      else if (name == "radius")
        data.radius = value;
      else if (name == "phi0")
        data.phi0 = value;
      else if (name == "symmetry")
        data.symmetry = value;
      else if (name == "systemID")
        data.systemID = value;
      else if (name == "encoding")
        data.encoding = ptr;
      else
        printout(dd4hep::WARNING, "DD4hep_CaloFaceBarrelSurfacePlugin", "unknown parameter:  %s ", name.c_str());
    }
  }
}

/// Install measurement surfaces
template <>
void Installer<CaloFaceBarrelSurfaces>::install(dd4hep::DetElement component, dd4hep::PlacedVolume pv) {
  /*if (data.symmetry)
      radius /= std::cos(M_PI / symmetry);*/
  const auto width = 2. * data.radius * std::tan(M_PI / data.symmetry);
  const auto inner_thickness = 1.e-6, outer_thickness = 1.e-6;

  printout(
      dd4hep::INFO, "DD4hep_CaloFaceBarrelSurfacePlugin", "install tracking surfaces for :  %s ", component.name());

  dd4hep::DDSegmentation::BitField64 bf("system:5,side:-2,layer:9,module:8,sensor:8");
  //dd4hep::DDSegmentation::BitField64 bf(data.encoding);

  bf["system"] = data.systemID;
  const auto alpha = data.symmetry ? 2. * M_PI / data.symmetry : 0.;
  for (int i = 0; i < data.symmetry; ++i) {
    bf["module"] = i;
    const auto gam = data.phi0 + alpha / 2. + i * alpha;

    dd4hep::rec::Vector3D u(std::cos(gam + M_PI / 2.), std::sin(gam + M_PI / 2.), 0.), v(0., 0., 1.),
        n(std::cos(gam), std::sin(gam), 0.), o(data.radius * std::cos(gam), data.radius * std::sin(gam), 0.);

    CaloBarrelPlane surf(
        pv.volume(), Type(Type::Helper, Type::Sensitive), inner_thickness, outer_thickness, u, v, n, o);
    surf->setData(data.length, width);
    surf->setID(bf.getValue());
    addSurface(component, surf);
  }
  stopScanning();  // stop scanning the hierarchy any further
}
