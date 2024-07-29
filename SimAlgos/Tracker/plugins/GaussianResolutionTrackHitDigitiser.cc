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
#include <DD4hep/VolumeManager.h>
#include <GaudiAlg/GaudiTool.h>
#include <GaudiKernel/IRndmGenSvc.h>
#include <GaudiKernel/RndmGenerators.h>
#include <edm4hep/MutableTrackerHit.h>
#include <edm4hep/SimTrackerHit.h>
#include <k4Interface/IGeoSvc.h>

#include "SimAlgos/Tracker/include/ITrackHitDigitisationAlgo.h"

class GaussianResolutionTrackHitDigitiser : public GaudiTool, virtual public ITrackHitDigitisationAlgo {
public:
  explicit GaussianResolutionTrackHitDigitiser(const std::string& type,
                                               const std::string& name,
                                               const IInterface* parent)
      : GaudiTool(type, name, parent),
        geom_("GeoSvc", name),
        readout_name_{this, "readoutName", ""},
        barrel_{this, "barrel", false},
        resol_u_{this, "uRes", 0.},
        resol_v_{this, "vRes", 0.},
        resol_time_{this, "timeRes", 0.} {}
  virtual ~GaussianResolutionTrackHitDigitiser() = default;

  inline StatusCode initialize() override {
    if (auto sc = GaudiTool::initialize(); sc.isFailure())
      return sc;
    auto rand_svc = svc<IRndmGenSvc>("RndmGenSvc", true);
    if (!rand_svc)
      return Error("Failed to retrieve the RndmGenSvc.");
    res_gen_u_ = rand_svc->generator(Rndm::Gauss(0., resol_u_));
    res_gen_v_ = rand_svc->generator(Rndm::Gauss(0., resol_v_));
    res_gen_time_ = rand_svc->generator(Rndm::Gauss(0., resol_time_));
    if (detector_ = geom_->getDetector(); !detector_)
      return Error("Failed to retrieve a detector from the geometry service.");
    volume_manager_ = detector_->volumeManager();
    decoder_ = detector_->readout(readout_name_).idSpec().decoder();
    return StatusCode::SUCCESS;
  }

  inline StatusCode run(const edm4hep::SimTrackerHit& simhit, edm4hep::MutableTrackerHit& hit) const override {
    const dd4hep::DDSegmentation::CellID cellid = simhit.getCellID();
    hit.setCellID(cellid);
    hit.setTime(simhit.getTime() + res_gen_time_->shoot());
    hit.setEDep(simhit.getEDep());


    const auto det_element = volume_manager_.lookupDetElement(cellid);
    const auto& simhit_pos = simhit.getPosition();
    const auto sim_global_pos =
        std::array<double, 3>{simhit_pos.x * dd4hep::mm, simhit_pos.y * dd4hep::mm, simhit_pos.z * dd4hep::mm};
    auto local_pos = std::array<double, 3>{0., 0., 0.}, digi_global_pos = local_pos;
    det_element.nominal().worldTransformation().MasterToLocal(sim_global_pos.data(), local_pos.data());
    // smear the local coordinates by the resolution
    if (barrel_) {
      local_pos[0] += res_gen_u_->shoot() * dd4hep::mm;
      local_pos[2] += res_gen_v_->shoot() * dd4hep::mm;
    } else {
      local_pos[0] += res_gen_u_->shoot() * dd4hep::mm;
      local_pos[1] += res_gen_v_->shoot() * dd4hep::mm;
    }
    det_element.nominal().worldTransformation().LocalToMaster(local_pos.data(), digi_global_pos.data());
    const auto hit_pos = edm4hep::Vector3d{
        digi_global_pos.at(0) / dd4hep::mm, digi_global_pos.at(1) / dd4hep::mm, digi_global_pos.at(2) / dd4hep::mm};
    hit.setPosition(hit_pos);

    //hit.setCovMatrix(std::array<float, 6>{});  //FIXME
    hit.addToRawHits(simhit.getObjectID());

    return StatusCode::SUCCESS;
  }

private:
  ServiceHandle<IGeoSvc> geom_;
  SmartIF<IRndmGen> res_gen_u_, res_gen_v_, res_gen_time_;

  Gaudi::Property<std::string> readout_name_;
  Gaudi::Property<bool> barrel_;
  Gaudi::Property<double> resol_u_, resol_v_, resol_time_;

  dd4hep::Detector* detector_{nullptr};
  dd4hep::DDSegmentation::BitFieldCoder* decoder_{nullptr};
  dd4hep::VolumeManager volume_manager_;
};

DECLARE_COMPONENT(GaussianResolutionTrackHitDigitiser)
