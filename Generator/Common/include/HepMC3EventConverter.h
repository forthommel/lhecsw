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

#ifndef Generator_Common_HepMC3EventConverter_h
#define Generator_Common_HepMC3EventConverter_h

#include <DDG4/Geant4InputAction.h>

namespace HepMC3 {
  class GenEvent;
}

using Particles = dd4hep::sim::Geant4InputAction::Particles;
using Vertices = dd4hep::sim::Geant4InputAction::Vertices;

class HepMC3EventConverter {
public:
  static bool convert(const HepMC3::GenEvent*, Vertices&, Particles&);
};

#endif
