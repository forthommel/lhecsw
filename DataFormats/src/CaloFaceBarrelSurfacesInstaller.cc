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

#include "DataFormats/include/CaloFaceBarrelSurfaces.h"
#include "Geometry/include/CaloBarrelPlane.h"

namespace {
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
    dd4hep::Volume comp_vol = pv.volume();

    double length = data.length;
    double symmetry = data.symmetry;
    double radius = data.radius;
    double phi0 = data.phi0;

    //    if( symmetry ) radius /= cos( M_PI / symmetry ) ;

    double width = 2. * radius * tan(M_PI / symmetry);

    double inner_thickness = 1e-6;
    double outer_thickness = 1e-6;

    printout(
        dd4hep::INFO, "DD4hep_CaloFaceBarrelSurfacePlugin", "install tracking surfaces for :  %s ", component.name());

    dd4hep::DDSegmentation::BitField64 bf("system:5,side:-2,layer:9,module:8,sensor:8");
    // dd4hep::DDSegmentation::BitField64 bf( data.encoding ) ;

    bf["system"] = data.systemID;

    const auto alpha = symmetry ? 2. * M_PI / symmetry : 0.;

    for (unsigned i = 0; i < symmetry; ++i) {
      bf["module"] = i;

      double gam = phi0 + alpha / 2. + i * alpha;

      dd4hep::rec::Vector3D u(cos(gam + M_PI / 2.), sin(gam + M_PI / 2.), 0.), v(0., 0., 1.), n(cos(gam), sin(gam), 0.),
          o(radius * cos(gam), radius * sin(gam), 0.);

      CaloBarrelPlane surf(comp_vol, Type(Type::Helper, Type::Sensitive), inner_thickness, outer_thickness, u, v, n, o);

      surf->setData(length, width);
      surf->setID(bf.getValue());

      addSurface(component, surf);
    }

    // stop scanning the hierarchy any further
    stopScanning();
  }
}  // namespace
