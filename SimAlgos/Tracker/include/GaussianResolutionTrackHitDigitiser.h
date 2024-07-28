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

#ifndef SimAlgos_Tracker_GaussianResolutionTrackHitDigitiser_h
#define SimAlgos_Tracker_GaussianResolutionTrackHitDigitiser_h

#include <DD4hep/Readout.h>
#include <DD4hep/VolumeManager.h>
#include <GaudiKernel/IRndmGenSvc.h>

#include "SimAlgos/Tracker/include/ITrackDigitisationAlgo.h"

class IGeoSvc;
namespace dd4hep::DDSegmentation {
  class BitFieldCoder;
  class CartesianGridXZ;
}  // namespace dd4hep::DDSegmentation

class GaussianResolutionTrackHitDigitiser : public ITrackDigitisationAlgo {
public:
  explicit GaussianResolutionTrackHitDigitiser(const std::string&, ISvcLocator*);
  virtual ~GaussianResolutionTrackHitDigitiser() = default;

  StatusCode initialize() override;
  StatusCode run(const edm4hep::SimTrackerHit&, edm4hep::MutableTrackerHit&) override;

private:
  ServiceHandle<IGeoSvc> geom_;
  SmartIF<IRndmGen> res_gen_;

  Gaudi::Property<std::string> readout_name_;

  dd4hep::Readout readout_;
  dd4hep::VolumeManager volume_manager_;
  dd4hep::DDSegmentation::BitFieldCoder* readout_decoder_{nullptr};
  dd4hep::DDSegmentation::CartesianGridXZ* segm_xz_{nullptr};

  double segm_grid_size_x_{0.}, segm_grid_size_z_{0.};
};

#endif
