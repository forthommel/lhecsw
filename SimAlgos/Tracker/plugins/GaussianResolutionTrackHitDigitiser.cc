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
#include <edm4hep/MutableTrackerHit.h>
#include <edm4hep/SimTrackerHit.h>
#include <k4Interface/IGeoSvc.h>

// art
#include <art/Framework/Core/EDProducer.h>

#include "SimAlgos/Tracker/include/ITrackHitDigitisationAlgo.h"

class GaussianResolutionTrackHitDigitiser : public art::EDProducer {
public:
  explicit GaussianResolutionTrackHitDigitiser(const fhicl::ParameterSet& iConfig)
      : art::EDProducer(iConfig),
        tracker_hits_token_{consumes<std::vector<edm4hep::SimCalorimeterHit> >()},
        readout_name_{iConfig.get<std::string>("readoutName")},
        is_barrel_{iConfig.get<bool>("isBarrel")},
        resol_u_{iConfig.get<double>("uRes")},
        resol_v_{iConfig.get<double>("vRes")},
        resol_time_{iConfig.get<double>("timeRes")} {
    art::ServiceHandle<art::RandomNumberGenerator> rng;
    if (random_engine_ = &rng->getEngine(); !random_engine_)
      return Error("Failed to retrieve the RndmGenSvc.");
    res_gen_u_ = std::make_unique<CLHEP::RandGauss>(random_engine_, 0., resol_u_);
    res_gen_v_ = std::make_unique<CLHEP::RandGauss>(random_engine_, 0., resol_v_);
    res_gen_time_ = std::make_unique<CLHEP::RandGauss>(random_engine_, 0., resol_time_);
    if (detector_ = geom_handle_->getDetector(); !detector_)
      return Error("Failed to retrieve a detector from the geometry service.");
    volume_manager_ = detector_->volumeManager();
    decoder_ = detector_->readout(readout_name_).idSpec().decoder();
  }
  virtual ~GaussianResolutionTrackHitDigitiser() = default;

  inline void analyze(art::Event& iEvent) override {
    auto hits = std::make_unique<std::vectror<dd4hep::MutableTrackerHit> >();
    for (const auto& simhit : iEvent.get(tracker_hits_token_)) {
      dd4hep::MutableTrackerHit hit;
      const dd4hep::DDSegmentation::CellID cellid = simhit.getCellID();
      hit.setCellID(cellid);
      hit.setTime(simhit.getTime() + res_gen_time_->fire());
      hit.setEDep(simhit.getEDep());

      const auto det_element = volume_manager_.lookupDetElement(cellid);
      const auto& simhit_pos = simhit.getPosition();
      const auto sim_global_pos =
          std::array<double, 3>{simhit_pos.x * dd4hep::mm, simhit_pos.y * dd4hep::mm, simhit_pos.z * dd4hep::mm};
      auto local_pos = std::array<double, 3>{0., 0., 0.}, digi_global_pos = local_pos;
      det_element.nominal().worldTransformation().MasterToLocal(sim_global_pos.data(), local_pos.data());
      // smear the local coordinates by the resolution
      if (is_barrel_) {
        local_pos[0] += res_gen_u_->fire() * dd4hep::mm;
        local_pos[2] += res_gen_v_->fire() * dd4hep::mm;
      } else {
        local_pos[0] += res_gen_u_->fire() * dd4hep::mm;
        local_pos[1] += res_gen_v_->fire() * dd4hep::mm;
      }
      det_element.nominal().worldTransformation().LocalToMaster(local_pos.data(), digi_global_pos.data());
      const auto hit_pos = edm4hep::Vector3d{
          digi_global_pos.at(0) / dd4hep::mm, digi_global_pos.at(1) / dd4hep::mm, digi_global_pos.at(2) / dd4hep::mm};
      hit.setPosition(hit_pos);

      //hit.setCovMatrix(std::array<float, 6>{});  //FIXME
      hit.addToRawHits(simhit.getObjectID());
      hits->emplace_back(hit);
    }
    iEvent.put(std::move(hits));
  }

private:
  const std::string readout_name_;
  const bool is_barrel_;
  const double resol_u_, resol_v_, resol_time_;

  art::ServiceHandle<IGeoSvc> geom_handle_;
  art::ProductToken<std::vector<edm4hep::SimTrackerHit> > tracker_hits_token_;

  CLHEP::HepRandomEngine* random_engine_{nullptr};
  std::unique_ptr<CLHEP::RandGauss> res_gen_u_, res_gen_v_, res_gen_time_;

  dd4hep::Detector* detector_{nullptr};
  dd4hep::DDSegmentation::BitFieldCoder* decoder_{nullptr};
  dd4hep::VolumeManager volume_manager_;
};

DECLARE_COMPONENT(GaussianResolutionTrackHitDigitiser)
