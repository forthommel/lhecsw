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

#ifndef SimAlgos_Tracker_TrivialTrackHitDigitiser_h
#define SimAlgos_Tracker_TrivialTrackHitDigitiser_h

#include <GaudiKernel/IRndmGenSvc.h>

#include "SimAlgos/Tracker/include/ITrackDigitisationAlgo.h"

class TrivialTrackHitDigitiser : public ITrackDigitisationAlgo {
public:
  explicit TrivialTrackHitDigitiser(const std::string&, ISvcLocator*);
  virtual ~TrivialTrackHitDigitiser() = default;

  StatusCode initialize() override;
  StatusCode run(const edm4hep::SimTrackerHit&, edm4hep::MutableTrackerHit&) override;

private:
  Gaudi::Property<float> x_res_, y_res_, z_res_;
  SmartIF<IRndmGen> x_res_gen_, y_res_gen_, z_res_gen_;
};

#endif
