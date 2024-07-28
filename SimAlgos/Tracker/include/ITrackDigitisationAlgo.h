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

#ifndef SimAlgos_Tracker_ITrackDigitisationAlgo_h
#define SimAlgos_Tracker_ITrackDigitisationAlgo_h

#include <GaudiAlg/GaudiAlgorithm.h>

namespace edm4hep {
  class SimTrackerHit;
  class MutableTrackerHit;
}  // namespace edm4hep

class ITrackDigitisationAlgo : public GaudiAlgorithm {
public:
  explicit ITrackDigitisationAlgo(const std::string& name, ISvcLocator* service_locator)
      : GaudiAlgorithm(name, service_locator) {}
  virtual ~ITrackDigitisationAlgo() = default;

  virtual StatusCode run(const edm4hep::SimTrackerHit&, edm4hep::MutableTrackerHit&) = 0;
};

#endif
