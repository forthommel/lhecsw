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

#include "SimG4/Common/include/SimRunManager.h"

SimRunManager::SimRunManager()
    : G4RunManager(), message_service_("MessageSvc", "SimRunManager"), log_(&(*message_service_), "SimRunManager") {}

StatusCode SimRunManager::initialize() {
  if (!G4RunManager::ConfirmBeamOnCondition())  // make sure we are in a /run/beamOn mode
    return StatusCode::FAILURE;
  G4RunManager::ConstructScoringWorlds();
  G4RunManager::RunInitialization();
  return StatusCode::SUCCESS;
}

StatusCode SimRunManager::finalize() {
  G4RunManager::RunTermination();
  return StatusCode::SUCCESS;  // can potentially check for Geant4 errors
}

StatusCode SimRunManager::run(G4Event& event, const std::function<bool(const G4Event&)>& callback) {
  G4RunManager::currentEvent = &event;
  G4RunManager::eventManager->ProcessOneEvent(G4RunManager::currentEvent);
  G4RunManager::AnalyzeEvent(G4RunManager::currentEvent);
  G4RunManager::UpdateScoring();
  if (callback)
    callback(event);
  G4RunManager::TerminateOneEvent();
  return StatusCode::SUCCESS;
}

const G4Event* SimRunManager::currentEvent() const { return G4RunManager::GetCurrentEvent(); }
