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

#include <GaudiAlg/GaudiAlgorithm.h>
#include <edm4hep/MCRecoTrackerAssociationCollection.h>
#include <edm4hep/SimTrackerHit.h>
#include <edm4hep/SimTrackerHitCollection.h>
#include <edm4hep/TrackerHit.h>
#include <edm4hep/TrackerHitCollection.h>
#include <k4FWCore/DataHandle.h>

#include "SimAlgos/Tracker/include/GaussianResolutionTrackHitDigitiser.h"
#include "SimAlgos/Tracker/include/TrivialTrackHitDigitiser.h"

class TrackHitDigitiser : public GaudiAlgorithm {
public:
  explicit TrackHitDigitiser(const std::string& name, ISvcLocator* service_locator)
      : GaudiAlgorithm(name, service_locator),
        simhits_coll_{"simhits", Gaudi::DataHandle::Reader, this},
        hits_coll_{name, Gaudi::DataHandle::Writer, this},
        simhits_hits_assoc_{name + "HitsAssociation", Gaudi::DataHandle::Writer, this},
        algo_name_{this, "algorithm", "trivial"} {
    declareProperty("simhits", simhits_coll_, "Input collection for simulated track hits");
    declareProperty(name, hits_coll_, "Output collection for digitised track hits");

    // build the user-steered hit digitiser flavour
    if (algo_name_ == "trivial")
      algo_.reset(new TrivialTrackHitDigitiser(name, service_locator));
    else if (algo_name_ == "gaussianResolution")
      algo_.reset(new GaussianResolutionTrackHitDigitiser(name, service_locator));
    else
      throw std::runtime_error("Invalid algorithm name: '" + algo_name_ + "'.");
  }
  virtual ~TrackHitDigitiser() = default;

  StatusCode initialize() override {
    if (const auto sc = GaudiAlgorithm::initialize(); sc.isFailure())
      return sc;
    return algo_->initialize();
  }
  StatusCode finalize() override { return GaudiAlgorithm::finalize(); }

  StatusCode execute() override {
    auto hits = hits_coll_.createAndPut();
    auto assocs = simhits_hits_assoc_.createAndPut();

    for (const auto& simhit : *simhits_coll_.get()) {
      auto hit = hits->create();  // new hit instance
      if (const auto sc = algo_->run(simhit, hit); sc.isFailure())
        return sc;

      auto assoc = assocs->create();  // simhit -> hit association
      assoc.setSim(simhit);
      assoc.setRec(hit);
      assoc.setWeight(1.);
    }
    return StatusCode::SUCCESS;
  }

private:
  // Inputs
  DataHandle<edm4hep::SimTrackerHitCollection> simhits_coll_;
  // Outputs
  DataHandle<edm4hep::TrackerHitCollection> hits_coll_;
  DataHandle<edm4hep::MCRecoTrackerAssociationCollection> simhits_hits_assoc_;

  Gaudi::Property<std::string> algo_name_;
  std::unique_ptr<ITrackDigitisationAlgo> algo_;
};

DECLARE_COMPONENT(TrackHitDigitiser)
