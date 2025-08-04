/*
 *  LHeC offline simulation and reconstruction software
 *  Copyright (C) 2024-2025  Laurent Forthomme
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

#include <Gaudi/Algorithm.h>
#include <GaudiKernel/ToolHandle.h>
#include <edm4hep/SimTrackerHit.h>
#include <edm4hep/SimTrackerHitCollection.h>
#include <edm4hep/TrackerHit3D.h>
#include <edm4hep/TrackerHit3DCollection.h>
#include <edm4hep/TrackerHitSimTrackerHitLinkCollection.h>
#include <k4FWCore/DataHandle.h>

#include "SimAlgos/Tracker/include/ITrackHitDigitisationAlgo.h"

class TrackHitDigitiser : public Gaudi::Algorithm {
public:
  explicit TrackHitDigitiser(const std::string& name, ISvcLocator* service_locator)
      : Gaudi::Algorithm(name, service_locator),
        simhits_coll_{"simhits", Gaudi::DataHandle::Reader, this},
        hits_coll_{name, Gaudi::DataHandle::Writer, this},
        simhits_hits_assoc_{name + "HitsAssociation", Gaudi::DataHandle::Writer, this} {
    declareProperty("algorithm", algo_, "digitisation algorithm flavour");
    declareProperty("simhits", simhits_coll_, "input collection for simulated track hits");
    declareProperty(name, hits_coll_, "output collection for digitised track hits");
  }
  virtual ~TrackHitDigitiser() = default;

  StatusCode initialize() override {
    if (const auto sc = Gaudi::Algorithm::initialize(); sc.isFailure())
      return sc;
    return algo_->initialize();
  }
  StatusCode finalize() override { return Gaudi::Algorithm::finalize(); }

  StatusCode execute(const EventContext&) const override {
    auto hits = hits_coll_.createAndPut();
    auto assocs = simhits_hits_assoc_.createAndPut();

    for (const auto& simhit : *simhits_coll_.get()) {
      auto hit = hits->create();  // new hit instance
      if (const auto sc = algo_->run(simhit, hit); sc.isFailure())
        return sc;

      auto assoc = assocs->create();  // simhit -> hit association
      assoc.setTo(simhit);
      assoc.setFrom(hit);
      assoc.setWeight(1.);
    }
    return StatusCode::SUCCESS;
  }

private:
  // Inputs
  mutable k4FWCore::DataHandle<edm4hep::SimTrackerHitCollection> simhits_coll_;
  // Outputs
  mutable k4FWCore::DataHandle<edm4hep::TrackerHit3DCollection> hits_coll_;
  mutable k4FWCore::DataHandle<edm4hep::TrackerHitSimTrackerHitLinkCollection> simhits_hits_assoc_;

  ToolHandle<ITrackHitDigitisationAlgo> algo_;  ///< user-steered hit digitiser flavour
};

DECLARE_COMPONENT(TrackHitDigitiser)
