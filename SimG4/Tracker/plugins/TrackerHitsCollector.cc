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
#include <DDG4/Geant4Data.h>
#include <DDG4/Geant4HitCollection.h>
#include <GaudiAlg/GaudiTool.h>
#include <edm4hep/Constants.h>
#include <edm4hep/SimTrackerHitCollection.h>
#include <k4FWCore/DataHandle.h>
#include <k4Interface/IGeoSvc.h>
#include <k4Interface/ISimG4SaveOutputTool.h>

#include <G4Event.hh>
#include <G4SystemOfUnits.hh>
#include <G4THitsCollection.hh>
#include <G4VHit.hh>

class TrackerHitsCollector : public GaudiTool, virtual public ISimG4SaveOutputTool {
public:
  explicit TrackerHitsCollector(const std::string& type, const std::string& name, const IInterface* parent)
      : GaudiTool(type, name, parent),
        geom_handle_{"GeoSvc", name},
        hits_handle_{"hits", Gaudi::DataHandle::Writer, this},
        readout_name_{this, "readoutName", "", "readout element name"} {
    declareInterface<ISimG4SaveOutputTool>(this);
  }
  virtual ~TrackerHitsCollector() {}

  /// Initialise the Geant4 simulation service
  StatusCode initialize() override {
    if (const auto status = GaudiTool::initialize(); status.isFailure())
      return status;
    if (readout_name_.empty())
      return Error("Failed to retrieve the readout element name.");

    // retrieve the geometry element
    if (!geom_handle_)
      return Error("Failed to retrieve the geometry handle.");
    const auto* detector = geom_handle_->getDetector();
    if (!detector)
      return Error("Failed to retrieve a detector from the geometry handle.");

    // ensure the readout element is part of the detector geometry
    if (detector->readouts().count(readout_name_) == 0)
      return Error("Failed to retrieve a readout component with name '" + readout_name_ +
                   "' from the geometry/detector.");
    info() << "Will build a '" << hits_handle_.objKey() << "' hit collection for '" << readout_name_
           << "' readout in the output file." << endmsg;

    return StatusCode::SUCCESS;
  }

  /// Save the event in the output file instance
  StatusCode saveOutput(const G4Event& iEvent) override {
    auto* hit_colls = iEvent.GetHCofThisEvent();  // should be constant, but API is rotten
    if (!hit_colls)
      return StatusCode::SUCCESS;  // skip event if no collection is present

    auto* hits = hits_handle_.createAndPut();
    for (int i = 0; i < hit_colls->GetNumberOfCollections(); ++i) {
      if (auto* collect = hit_colls->GetHC(i); collect->GetName() == readout_name_) {
        if (auto* g4hits = dynamic_cast<dd4hep::sim::Geant4HitCollection*>(collect); g4hits) {
          verbose() << "Tracker collection '" << collect->GetName() << "' (#" << i << ") has " << collect->GetSize()
                    << " hit(s) stored.";
          for (const auto& trk_hit : g4hits->getHits<dd4hep::sim::Geant4Tracker::Hit>())
            parseHit(trk_hit, hits);
        } /* else if (const auto* const_g4hits = dynamic_cast<const dd4hep::sim::Geant4HitCollection*>(collect);
                   const_g4hits) {
          auto* g4hits = const_cast<dd4hep::sim::Geant4HitCollection*>(const_g4hits);  // again, fantastic API
          if (!g4hits)
            return Error("Failed to const-cast dd4hep object.");
          verbose() << "Tracker collection '" << collect->GetName() << "' (#" << i << ") has " << collect->GetSize()
                    << " hit(s) stored.";
          for (const auto& trk_hit : g4hits->getHits<dd4hep::sim::Geant4Tracker::Hit>())
            parseHit(trk_hit, hits);
        } else if (const auto* g4hits_coll =
                       dynamic_cast<const G4THitsCollection<dd4hep::sim::Geant4Tracker::Hit*>*>(collect);
                   g4hits_coll) {
          for (const auto* trk_hit : *g4hits_coll->GetVector())
            parseHit(*trk_hit, hits);
        } else if (const auto* vec_g4hits = dynamic_cast<const std::vector<dd4hep::sim::Geant4Tracker::Hit*>*>(collect);
                   vec_g4hits) {
          for (const auto* trk_hit : *vec_g4hits)
            parseHit(trk_hit, hits);
        }*/
        else
          return Error("Failed to cast the Geant4 hits collection '" + collect->GetName() + "' (#" +
                       std::to_string(collect->GetColID()) + ") from '" + collect->GetSDname() +
                       "' SD into a dd4hep object.");
      }
    }
    return StatusCode::SUCCESS;
  }

private:
  void parseHit(const dd4hep::sim::Geant4Tracker::Hit* trk_hit, edm4hep::SimTrackerHitCollection* hits) const {
    auto hit = hits->create();  // mutable
    hit.setCellID(trk_hit->cellID);
    hit.setEDep(trk_hit->energyDeposit * CLHEP::MeV / CLHEP::GeV);
    hit.setQuality(trk_hit->truth.trackID);  // store trackid in an unrelated field
    hit.setTime(trk_hit->truth.time);
    hit.setPosition({trk_hit->position.x(), trk_hit->position.y(), trk_hit->position.z()});
    hit.setMomentum({(float)trk_hit->momentum.x(), (float)trk_hit->momentum.y(), (float)trk_hit->momentum.z()});
    hit.setPathLength(trk_hit->length);
  }

  ServiceHandle<IGeoSvc> geom_handle_;
  DataHandle<edm4hep::SimTrackerHitCollection> hits_handle_;

  Gaudi::Property<std::string> readout_name_;
};

DECLARE_COMPONENT(TrackerHitsCollector)
