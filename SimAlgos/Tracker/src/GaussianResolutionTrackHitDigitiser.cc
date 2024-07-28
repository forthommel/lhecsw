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

#include <DD4hep/Detector.h>
#include <DDSegmentation/CartesianGridXZ.h>
#include <GaudiKernel/RndmGenerators.h>
#include <edm4hep/MutableTrackerHit.h>
#include <edm4hep/SimTrackerHit.h>
#include <k4Interface/IGeoSvc.h>

#include "SimAlgos/Tracker/include/GaussianResolutionTrackHitDigitiser.h"

GaussianResolutionTrackHitDigitiser::GaussianResolutionTrackHitDigitiser(const std::string& name,
                                                                         ISvcLocator* service_locator)
    : ITrackDigitisationAlgo(name, service_locator), geom_("GeoSvc", name), readout_name_{this, "readout", ""} {}

StatusCode GaussianResolutionTrackHitDigitiser::initialize() {
  if (auto sc = ITrackDigitisationAlgo::initialize(); sc.isFailure())
    return sc;
  auto* detector = geom_->getDetector();
  if (!detector)
    return Error("Failed to retrieve a detector from the geometry service.");
  readout_ = detector->readout(readout_name_);
  if (readout_decoder_ = readout_.idSpec().decoder(); !readout_decoder_)
    return Error("Failed to retrieve the readout decoder for readout name '" + readout_name_ + "'.");
  if (segm_xz_ = dynamic_cast<dd4hep::DDSegmentation::CartesianGridXZ*>(readout_.segmentation().segmentation());
      !segm_xz_)
    return Error("Failed to initialise the x-z segmentation for readout name '" + readout_name_ + "'.");
  segm_grid_size_x_ = segm_xz_->gridSizeX();
  segm_grid_size_z_ = segm_xz_->gridSizeZ();
  volume_manager_ = detector->volumeManager();
  res_gen_ = randSvc()->generator(Rndm::Gauss(0., std::sqrt(1. / 12.)));
  return StatusCode::SUCCESS;
}

StatusCode GaussianResolutionTrackHitDigitiser::run(const edm4hep::SimTrackerHit& simhit,
                                                    edm4hep::MutableTrackerHit& hit) {
  const auto cellid = simhit.getCellID();
  hit.setCellID(cellid);
  hit.setTime(simhit.getTime());
  hit.setEDep(simhit.getEDep());

  const auto ix = readout_decoder_->get(cellid, "x"),
             iz = readout_decoder_->get(cellid, "z");  //TODO: implement caching?
  const auto& det_element = volume_manager_.lookupDetElement(cellid);
  const auto local_pos = std::array<double, 3>{
      (ix + res_gen_->shoot()) * segm_grid_size_x_, 0., (iz + res_gen_->shoot()) * segm_grid_size_z_};
  auto global_pos = std::array<double, 3>{0.};
  det_element.nominal().worldTransformation().LocalToMaster(local_pos.data(),
                                                            global_pos.data());  // local->global coords
  hit.setPosition(edm4hep::Vector3d{global_pos.data()});

  hit.setCovMatrix(std::array<float, 6>{});  //FIXME
  hit.addToRawHits(simhit.getObjectID());

  return StatusCode::SUCCESS;
}
