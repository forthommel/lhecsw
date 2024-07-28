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

#include <GaudiKernel/RndmGenerators.h>
#include <edm4hep/MutableTrackerHit.h>
#include <edm4hep/SimTrackerHit.h>

#include "SimAlgos/Tracker/include/TrivialTrackHitDigitiser.h"

TrivialTrackHitDigitiser::TrivialTrackHitDigitiser(const std::string& name, ISvcLocator* service_locator)
    : ITrackDigitisationAlgo(name, service_locator),
      x_res_{this, "xResolution", 0.},
      y_res_{this, "yResolution", 0.},
      z_res_{this, "zResolution", 0.} {}

StatusCode TrivialTrackHitDigitiser::initialize() {
  if (auto sc = ITrackDigitisationAlgo::initialize(); sc.isFailure())
    return sc;
  x_res_gen_ = randSvc()->generator(Rndm::Gauss(0., x_res_));
  y_res_gen_ = randSvc()->generator(Rndm::Gauss(0., y_res_));
  z_res_gen_ = randSvc()->generator(Rndm::Gauss(0., z_res_));
  return StatusCode::SUCCESS;
}

StatusCode TrivialTrackHitDigitiser::run(const edm4hep::SimTrackerHit& simhit, edm4hep::MutableTrackerHit& hit) {
  hit.setCellID(simhit.getCellID());
  hit.setTime(simhit.getTime());
  hit.setEDep(simhit.getEDep());

  const auto& hit_pos = simhit.getPosition();
  // smear hit position according to cartesian coordinates resolution
  hit.setPosition(edm4hep::Vector3d{
      hit_pos[0] + x_res_gen_->shoot(), hit_pos[1] + y_res_gen_->shoot(), hit_pos[2] + z_res_gen_->shoot()});
  hit.setCovMatrix(std::array<float, 6>{});  //FIXME
  hit.addToRawHits(simhit.getObjectID());

  return StatusCode::SUCCESS;
}
