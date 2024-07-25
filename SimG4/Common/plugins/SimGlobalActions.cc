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

#include <GaudiKernel/AlgTool.h>
#include <k4Interface/IGeoSvc.h>
#include <k4Interface/ISimG4ActionTool.h>

#include "SimG4/Common/include/SimG4ActionInitialization.h"

class SimGlobalActions final : public AlgTool, public ISimG4ActionTool {
public:
  explicit SimGlobalActions(const std::string&, const std::string&, const IInterface*);
  virtual ~SimGlobalActions() {}

  StatusCode initialize() override;

  G4VUserActionInitialization* userActionInitialization() override;

private:
  ServiceHandle<IGeoSvc> geometry_service_;
};

DECLARE_COMPONENT(SimGlobalActions)

SimGlobalActions::SimGlobalActions(const std::string& type, const std::string& name, const IInterface* parent)
    : AlgTool(type, name, parent), geometry_service_("GeoSvc", name) {
  declareInterface<ISimG4ActionTool>(this);
}

StatusCode SimGlobalActions::initialize() {
  if (AlgTool::initialize().isFailure())
    return StatusCode::FAILURE;
  if (!geometry_service_) {
    error() << "Unable to initialise the geometry service.";
    return StatusCode::FAILURE;
  }
  return StatusCode::SUCCESS;
}

G4VUserActionInitialization* SimGlobalActions::userActionInitialization() {
  auto* actions = new SimG4ActionInitialization();
  //actions->setSegmentation(pSeg);
  //actions->setBirksConstant(m_scintName, m_birks);
  //actions->setThreshold(m_thres);
  return actions;
}
