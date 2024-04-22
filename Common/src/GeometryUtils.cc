#include <DD4hep/DetElement.h>

#include <cmath>

#include "Common/include/GeometryUtils.h"

namespace utils {
  double computeDpt(double ra, double rb, double theta) {
    return std::hypot(ra * std::sin(theta), rb * std::cos(theta));
  }

  void placeStaves(dd4hep::DetElement& parent,
                   const dd4hep::DetElement& stave,
                   double rmin,
                   int numsides,
                   double total_thickness,
                   dd4hep::Volume& envelopeVolume,
                   double innerAngle,
                   const dd4hep::Volume& sectVolume) {
    double innerRotation = innerAngle;
    double offsetRotation = -innerRotation / 2;
    double sectCenterRadius = rmin + total_thickness / 2;
    double rotX = M_PI / 2;
    double rotY = -offsetRotation;
    double posX = -sectCenterRadius * std::sin(rotY);
    double posY = sectCenterRadius * std::cos(rotY);

    for (int module = 1; module <= numsides; ++module) {
      dd4hep::DetElement det = module > 1 ? stave.clone(dd4hep::_toString(module, "stave%d")) : stave;
      dd4hep::Transform3D trafo(dd4hep::RotationZYX(0, rotY, rotX), dd4hep::Translation3D(-posX, -posY, 0));
      dd4hep::PlacedVolume pv = envelopeVolume.placeVolume(sectVolume, trafo);
      // Not a valid volID: pv.addPhysVolID("stave", 0);
      pv.addPhysVolID("module", module);
      det.setPlacement(pv);
      parent.add(det);
      rotY -= innerRotation;
      posX = -sectCenterRadius * std::sin(rotY);
      posY = sectCenterRadius * std::cos(rotY);
    }
  }
}  // namespace utils
