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
        resol_energy_{this, "energyResolution", 0., "energy relative resolution on readout"} {}
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

    if (const auto ene = simhit.getEnergy(); ene > 0.)
      hit.setEnergy(std::fabs(ene + res_gen_->shoot() * resol_energy_));
    else {
      warning() << "SimHit with invalid energy: E=" << ene << " <= 0.";
      hit.setEnergy(ene);
    }

    return StatusCode::SUCCESS;
  }

private:
  ServiceHandle<IGeoSvc> geom_;
  SmartIF<IRndmGen> res_gen_;

  Gaudi::Property<std::string> readout_name_;
  Gaudi::Property<double> resol_energy_;

  dd4hep::Detector* detector_{nullptr};
  dd4hep::DDSegmentation::BitFieldCoder* decoder_{nullptr};
  dd4hep::VolumeManager volume_manager_;
};

DECLARE_COMPONENT(GaussianResolutionCaloHitDigitiser)
