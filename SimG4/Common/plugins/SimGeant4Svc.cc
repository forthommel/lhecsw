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

#include <GaudiKernel/IRndmEngine.h>
#include <GaudiKernel/IRndmGenSvc.h>
#include <GaudiKernel/Service.h>
#include <GaudiKernel/ToolHandle.h>

#include <G4UImanager.hh>
#include <G4UIsession.hh>

#include "SimG4/Common/include/ISimGeant4DetectorConstruction.h"
#include "SimG4/Common/include/ISimGeant4Svc.h"
#include "SimG4/Common/include/SimRunManager.h"

class SimGeant4Svc final : public extends1<Service, ISimGeant4Svc> {
public:
  explicit SimGeant4Svc(const std::string& name, ISvcLocator* service_locator)
      : base_class(name, service_locator),
        detector_tool_{"SimGeant4DD4hepDetector", this, true},
        cmds_preinit_{this, "g4PreInitCommands", {}, "Geant4 commands to be parsed before kernel initialisation"},
        cmds_postinit_{this, "g4PostInitCommands", {}, "Geant4 commands to be parsed after kernel initialisation"},
        use_gaudi_rng_{
            this, "randomNumbersFromGaudi", true, "use Gaudi (true) or CLHEP (false) random number generator"},
        rng_seed_{this, "seedValue", 42, "RndmGenSvc (non-Gaudi) engine seed"} {
    declareProperty("detector", detector_tool_, "Detector construction tool handle");
  }
  virtual ~SimGeant4Svc() {}

  StatusCode initialize() override;  ///< Initialise the Geant4 simulation service
  StatusCode finalize() override;    ///< Concludes the Geant4 simulation service

  /// Process one event
  StatusCode run(G4Event& event, const std::function<bool(const G4Event&)>& callback) {
    if (const auto status = run_manager_.run(event, callback); status)
      return status;
    error() << "Failed to process the event in Geant4 run manager.";
    return StatusCode::FAILURE;
  }
  /// Retrieve the last processed event
  const G4Event* currentEvent() const { return run_manager_.currentEvent(); }

private:
  SmartIF<IRndmGenSvc> rng_service_;                          ///< Random number generator service
  ToolHandle<ISimGeant4DetectorConstruction> detector_tool_;  ///< DD4hep-to-Geant4 detector construction tool

  Gaudi::Property<std::vector<std::string> > cmds_preinit_;   ///< Geant4 commands applied before kernel initialisation
  Gaudi::Property<std::vector<std::string> > cmds_postinit_;  ///< Geant4 commands applied after kernel initialisation
  Gaudi::Property<bool> use_gaudi_rng_;                       ///< Retrieve random number generator seeds from Gaudi?
  Gaudi::Property<long> rng_seed_;                            ///< Random number generator seed (non-Gaudi)

  SimRunManager run_manager_;  ///< A Geant4 run manager for this instance

  std::unique_ptr<G4UIsession> session_;  ///< Geant4 interactive session handle
};

DECLARE_COMPONENT(SimGeant4Svc)

StatusCode SimGeant4Svc::initialize() {
  if (Service::initialize().isFailure()) {  // initialise Gaudi components
    error() << "Failed to initialise the Gaudi service." << endmsg;
    return StatusCode::FAILURE;
  }
  if (rng_service_ = service("RndmGenSvc"); !rng_service_) {
    error() << "Failed to retrieve the RndmGen service from configuration." << endmsg;
    return StatusCode::FAILURE;
  }
  if (!detector_tool_.retrieve()) {
    error() << "Failed to retrieve the detector construction tool" << endmsg;
    return StatusCode::FAILURE;
  }
  //run_manager_.SetUserInitialization(m_physicsListTool->physicsList());
  run_manager_.SetUserInitialization(detector_tool_->detectorConstruction());

  auto* g4_ui_manager = G4UImanager::GetUIpointer();  // retrieve pointer to the Geant4 UI

  for (const auto& cmd : cmds_preinit_)  // pre-initialisation kernel commands
    g4_ui_manager->ApplyCommand(cmd);

  run_manager_.Initialize();

  for (const auto& cmd : cmds_postinit_)  // post-initialisation kernel commands
    g4_ui_manager->ApplyCommand(cmd);

  std::vector<long> seedsVec;
  if (use_gaudi_rng_) {
    rng_service_->engine()->seeds(seedsVec).ignore();
    std::array<long, 2> seedsList{seedsVec[0], seedsVec[1]};
    CLHEP::HepRandom::setTheSeeds(seedsList.data());
    info() << "Random numbers seeds: " << CLHEP::HepRandom::getTheSeeds()[0] << ", "
           << CLHEP::HepRandom::getTheSeeds()[1] << endmsg;
  } else {
    rng_service_->engine()->setSeeds({rng_seed_}).ignore();
    rng_service_->engine()->seeds(seedsVec).ignore();
    info() << "Random numbers seeds: " << seedsVec << endmsg;
  }
  return StatusCode::SUCCESS;
}

StatusCode SimGeant4Svc::finalize() {
  if (!run_manager_.finalize()) {
    error() << "Failed to finalise the run manager.";
    return StatusCode::FAILURE;
  }
  return Service::finalize();
}
