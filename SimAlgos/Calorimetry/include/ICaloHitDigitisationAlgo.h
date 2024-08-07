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

#ifndef SimAlgos_Calorimetry_ICaloHitDigitisationAlgo_h
#define SimAlgos_Calorimetry_ICaloHitDigitisationAlgo_h

#include <GaudiKernel/IAlgTool.h>

namespace edm4hep {
  class SimCalorimeterHit;
  class MutableCalorimeterHit;
}  // namespace edm4hep

class ICaloHitDigitisationAlgo : virtual public IAlgTool {
public:
  DeclareInterfaceID(ICaloHitDigitisationAlgo, 1, 0);

  virtual StatusCode run(const edm4hep::SimCalorimeterHit&, edm4hep::MutableCalorimeterHit&) const = 0;
};

#endif
