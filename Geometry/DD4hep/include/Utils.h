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

#ifndef Geometry_DD4hep_Utils_h
#define Geometry_DD4hep_Utils_h

namespace dd4hep {
  class DetElement;
  class Volume;
}  // namespace dd4hep

namespace utils {
  /// calculate path in a given theta
  double computeDpt(double ra, double rb, double theta);
  void placeStaves(dd4hep::DetElement& parent,
                   const dd4hep::DetElement& stave,
                   double rmin,
                   int numsides,
                   double total_thickness,
                   dd4hep::Volume& envelopeVolume,
                   double innerAngle,
                   const dd4hep::Volume& sectVolume);
}  // namespace utils

#endif
