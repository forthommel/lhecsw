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
#include <edm4hep/MutableCalorimeterHit.h>
#include <edm4hep/SimCalorimeterHit.h>
#include <k4Interface/IGeoSvc.h>

#include "SimAlgos/Calorimetry/include/ICaloHitDigitisationAlgo.h"

class GaussianResolutionCaloHitDigitiser : public GaudiTool, virtual public ICaloHitDigitisationAlgo {
public:
  explicit GaussianResolutionCaloHitDigitiser(const std::string& type,
                                              const std::string& name,
                                              const IInterface* parent)
      : GaudiTool(type, name, parent),
        geom_("GeoSvc", name),
        readout_name_{this, "readoutName", ""},
        is_barrel_{this, "isBarrel", false},
        resol_a_{this, "aTerm", 0., "stochastic term for relative resolution"},
        resol_b_{this, "bTerm", 0., "constant term for relative resolution"} {}
  virtual ~GaussianResolutionCaloHitDigitiser() = default;

  inline StatusCode initialize() override {
    if (auto sc = GaudiTool::initialize(); sc.isFailure())
      return sc;
    auto rand_svc = svc<IRndmGenSvc>("RndmGenSvc", true);
    if (!rand_svc)
      return Error("Failed to retrieve the RndmGenSvc.");
    res_gen_ = rand_svc->generator(Rndm::Gauss(0., 1.));
    if (detector_ = geom_->getDetector(); !detector_)
      return Error("Failed to retrieve a detector from the geometry service.");
    volume_manager_ = detector_->volumeManager();
    decoder_ = detector_->readout(readout_name_).idSpec().decoder();
    return StatusCode::SUCCESS;
  }

  inline StatusCode run(const edm4hep::SimCalorimeterHit& simhit, edm4hep::MutableCalorimeterHit& hit) const override {
    const dd4hep::DDSegmentation::CellID cellid = simhit.getCellID();
    hit.setCellID(cellid);
    hit.setPosition(simhit.getPosition());  //FIXME also smear the position

    const auto ene = simhit.getEnergy();  //FIXME
    const auto smeared_ene =
        ene * std::fabs(res_gen_->shoot() /*FIXME*/) * std::hypot(resol_a_ / std::sqrt(ene), resol_b_);
    hit.setEnergy(smeared_ene);
    //hit.addToRawHits(simhit.getObjectID());

    return StatusCode::SUCCESS;
  }

private:
  ServiceHandle<IGeoSvc> geom_;
  SmartIF<IRndmGen> res_gen_;

  Gaudi::Property<std::string> readout_name_;
  Gaudi::Property<bool> is_barrel_;
  Gaudi::Property<double> resol_a_, resol_b_;

  dd4hep::Detector* detector_{nullptr};
  dd4hep::DDSegmentation::BitFieldCoder* decoder_{nullptr};
  dd4hep::VolumeManager volume_manager_;
};

DECLARE_COMPONENT(GaussianResolutionCaloHitDigitiser)
