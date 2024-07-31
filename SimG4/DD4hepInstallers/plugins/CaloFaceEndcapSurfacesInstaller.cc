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

#include "DataFormats/SimDD4hep/include/CaloFaceEndcapSurfaces.h"
#include "Geometry/DD4hep/include/CaloEndcapPlane.h"

template <>
void Installer<CaloFaceEndcapSurfaces>::handle_arguments(int argc, char** argv) {
  for (int i = 0; i < argc; ++i) {
    if (char* ptr = ::strchr(argv[i], '='); ptr) {
      const std::string name(argv[i], ptr);
      const auto value = dd4hep::_toDouble(++ptr);

      printout(dd4hep::DEBUG, "DD4hep_CaloFaceEndcapSurfacePlugin", "argument[%d] = %s = %f", i, name.c_str(), value);

      if (name == "zpos")
        data.zpos = value;
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
        printout(dd4hep::WARNING, "DD4hep_CaloFaceEndcapSurfacePlugin", "unknown parameter:  %s ", name.c_str());
    }
  }
}

/// Install measurement surfaces
template <>
void Installer<CaloFaceEndcapSurfaces>::install(dd4hep::DetElement component, dd4hep::PlacedVolume pv) {
  dd4hep::Volume comp_vol = pv.volume();

  const auto radius = data.symmetry ? data.radius / std::cos(M_PI / data.symmetry) : data.radius;
  const auto inner_thickness = 1.e-6, outer_thickness = 1.e-6;

  printout(
      dd4hep::INFO, "DD4hep_CaloFaceEndcapSurfacePlugin", "install tracking surfaces for :  %s ", component.name());

  dd4hep::DDSegmentation::BitField64 bf("system:5,side:-2,layer:9,module:8,sensor:8");
  //dd4hep::DDSegmentation::BitField64 bf( data.encoding ) ;

  bf["system"] = data.systemID;
  bf["side"] = 1;

  // shift position of origin of helper plane to pick up Air instead of vacuum
  const dd4hep::rec::Vector3D u(1., 0., 0.), v(0., 1., 0.), n(0., 0., 1.), o(0., 0.5 * radius, data.zpos);

  CaloEndcapPlane surf_pz(comp_vol, Type(Type::Helper, Type::Sensitive), inner_thickness, outer_thickness, u, v, n, o);
  surf_pz->setData(radius, data.phi0, data.symmetry);
  surf_pz->setID(bf.getValue());
  addSurface(component, surf_pz);

  CaloEndcapPlane surf_nz(
      comp_vol, Type(Type::Helper, Type::Sensitive), inner_thickness, outer_thickness, u, v, n, -1 * o);
  bf["side"] = -1;
  surf_nz->setData(radius, data.phi0, data.symmetry);
  surf_nz->setID(bf.getValue());
  addSurface(component, surf_nz);

  stopScanning();  // stop scanning the hierarchy any further
}
