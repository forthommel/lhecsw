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

#ifndef SimG4_SimGeant4Svc_h
#define SimG4_SimGeant4Svc_h

#include <GaudiKernel/IRndmEngine.h>
#include <GaudiKernel/IRndmGenSvc.h>
#include <GaudiKernel/Service.h>
#include <GaudiKernel/ToolHandle.h>

#include <G4UIsession.hh>

#include "SimG4/include/ISimGeant4DetectorConstruction.h"
#include "SimG4/include/ISimGeant4Svc.h"
#include "SimG4/include/SimRunManager.h"

class SimGeant4Svc final : public extends1<Service, ISimGeant4Svc> {
public:
  explicit SimGeant4Svc(const std::string&, ISvcLocator*);
  virtual ~SimGeant4Svc() {}

  StatusCode initialize() override;  ///< Initialise the Geant4 simulation service
  StatusCode finalize() override;    ///< Concludes the Geant4 simulation service

  StatusCode run(G4Event&, const std::function<bool(const G4Event&)>&);  ///< Process one event
  const G4Event* currentEvent() const;                                   ///< Retrieve the last processes event

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

#endif
