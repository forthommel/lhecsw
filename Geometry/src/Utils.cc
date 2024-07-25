/*
 *  LHeC offline simulation and reconstruction software
 *  Copyright (C) 2024  Laurent Forthomme
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <DD4hep/DetElement.h>

#include <cmath>

#include "Geometry/include/Utils.h"

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
