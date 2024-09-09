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
#include <edm4hep/MutableCalorimeterHit.h>
#include <edm4hep/SimCalorimeterHit.h>

// art
#include <art/Framework/Core/EDProducer.h>
#include <art/Framework/Principal/Handle.h>
#include <art/Framework/Services/Optional/RandomNumberGenerator.h>
#include <art/Framework/Services/Registry/ServiceHandle.h>

#include "SimAlgos/Calorimetry/include/ICaloHitDigitisationAlgo.h"

class GaussianResolutionCaloHitDigitiser : public art::EDProducer {
public:
  explicit GaussianResolutionCaloHitDigitiser(const fhicl::ParameterSet& iConfig)
      : art::EDProducer(iConfig),
        calo_hits_token_{consumes<std::vector<edm4hep::SimCalorimeterHit> >()},
        readout_name_{iConfig.get<std::string>("readoutName")},
        is_barrel_{iConfig.get<bool>("isBarrel")},
        resol_a_{iConfig.get<double>("aTerm")},
        resol_b_{iConfig.get<double>("bTerm")} {
    produces<std::vector<dd4hep::MutableCalorimeterHit> >();
    art::ServiceHandle<art::RandomNumberGenerator> rng;
    random_engine_ = &rng->getEngine();
  }
  virtual ~GaussianResolutionCaloHitDigitiser() = default;

  inline void beginRun(art::Run&) override {
    auto rand_svc = svc<IRndmGenSvc>("RndmGenSvc", true);
    if (!rand_svc)
      return Error("Failed to retrieve the RndmGenSvc.");
    res_gen_ = rand_svc->generator(Rndm::Gauss(0., 1.));
    if (detector_ = geom_handle_->getDetector(); !detector_)
      return Error("Failed to retrieve a detector from the geometry service.");
    volume_manager_ = detector_->volumeManager();
    decoder_ = detector_->readout(readout_name_).idSpec().decoder();
  }

  inline void produce(art::Event& iEvent) override {
    auto hits = std::make_unique<std::vectror<dd4hep::MutableCalorimeterHit> >();
    for (const auto& simhit : iEvent.get(calo_hits_token_)) {
      const dd4hep::DDSegmentation::CellID cellid = simhit.getCellID();
      dd4hep::MutableCalorimeterHit hit;
      hit.setCellID(cellid);
      hit.setPosition(simhit.getPosition());  //FIXME also smear the position

      if (const auto ene = simhit.getEnergy(); ene > 0.) {
        const auto dene = std::hypot(resol_a_ / std::sqrt(ene), resol_b_);
        const auto smeared_ene = std::fabs(ene + res_gen_->shoot() * ene * dene);
        hit.setEnergy(smeared_ene);
      } else {
        warning() << "SimHit with invalid energy: E=" << ene << " <= 0.";
        hit.setEnergy(ene);
      }
      hits->emplace_back(hit);
    }
    iEvent.put(std::move(hits));
  }

private:
  art::ServiceHandle<IGeoSvc> geom_handle_;
  art::ProductToken<std::vector<edm4hep::SimCalorimeterHit> > calo_hits_token_;

  const std::string readout_name_;
  const bool is_barrel_;
  const double resol_a_, resol_b_;

  CLHEP::HepRandomEngine* random_engine_{nullptr};

  dd4hep::Detector* detector_{nullptr};
  dd4hep::DDSegmentation::BitFieldCoder* decoder_{nullptr};
  dd4hep::VolumeManager volume_manager_;
};

DECLARE_COMPONENT(GaussianResolutionCaloHitDigitiser)
