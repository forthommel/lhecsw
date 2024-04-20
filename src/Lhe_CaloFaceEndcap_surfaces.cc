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

#include <string>

namespace {
  struct UserData {
    float zpos;
    float radius;
    float phi0;
    int symmetry;
    int systemID;
    std::string encoding;
  };
}  // namespace

// Framework include files
#define SURFACEINSTALLER_DATA UserData
#define DD4HEP_USE_SURFACEINSTALL_HELPER DD4hep_CaloFaceEndcapSurfacePlugin
#include <DD4hep/Printout.h>
#include <DD4hep/SurfaceInstaller.h>
#include <DDRec/DetectorData.h>
#include <DDRec/Surface.h>
#include <DDSegmentation/BitField64.h>

#include "Geometry/include/CaloEndcapPlane.h"

namespace {
  template <>
  void Installer<UserData>::handle_arguments(int argc, char** argv) {
    for (int i = 0; i < argc; ++i) {
      char* ptr = ::strchr(argv[i], '=');
      if (ptr) {
        std::string name(argv[i], ptr);
        double value = dd4hep::_toDouble(++ptr);

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
        else {
          printout(dd4hep::WARNING, "DD4hep_CaloFaceEndcapSurfacePlugin", "unknown parameter:  %s ", name.c_str());
        }
      }
    }
  }

  /// Install measurement surfaces
  template <typename UserData>
  void Installer<UserData>::install(dd4hep::DetElement component, dd4hep::PlacedVolume pv) {
    dd4hep::Volume comp_vol = pv.volume();

    double zpos = data.zpos;
    double symmetry = data.symmetry;
    double radius = data.radius;
    double phi0 = data.phi0;

    if (symmetry)
      radius /= cos(M_PI / symmetry);

    double inner_thickness = 1e-6;
    double outer_thickness = 1e-6;

    printout(
        dd4hep::INFO, "DD4hep_CaloFaceEndcapSurfacePlugin", "install tracking surfaces for :  %s ", component.name());

    dd4hep::DDSegmentation::BitField64 bf("system:5,side:-2,layer:9,module:8,sensor:8");
    //dd4hep::DDSegmentation::BitField64 bf( data.encoding ) ;

    bf["system"] = data.systemID;
    bf["side"] = 1;

    // shift position of origin of helper plane to pick up Air instead of vacuum
    dd4hep::rec::Vector3D u(1., 0., 0.), v(0., 1., 0.), n(0., 0., 1.), o(0., 0.5 * radius, zpos);

    CaloEndcapPlane surf_pz(
        comp_vol, Type(Type::Helper, Type::Sensitive), inner_thickness, outer_thickness, u, v, n, o);

    surf_pz->setData(radius, phi0, symmetry);
    surf_pz->setID(bf.getValue());

    addSurface(component, surf_pz);

    CaloEndcapPlane surf_nz(
        comp_vol, Type(Type::Helper, Type::Sensitive), inner_thickness, outer_thickness, u, v, n, -1 * o);

    bf["side"] = -1;
    surf_nz->setData(radius, phi0, symmetry);
    surf_nz->setID(bf.getValue());

    addSurface(component, surf_nz);

    // stop scanning the hierarchy any further
    stopScanning();
  }

}  // namespace
