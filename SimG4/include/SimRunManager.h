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

#ifndef SimG4_SimRunManager_h
#define SimG4_SimRunManager_h

#include <GaudiKernel/IMessageSvc.h>
#include <GaudiKernel/MsgStream.h>
#include <GaudiKernel/ServiceHandle.h>

#include <G4RunManager.hh>

class SimRunManager : public G4RunManager {
public:
  explicit SimRunManager();
  virtual ~SimRunManager() {}

  StatusCode initialize();
  StatusCode finalize();

  StatusCode run(G4Event&, const std::function<bool(const G4Event&)>&);
  const G4Event* currentEvent() const;

private:
  ServiceHandle<IMessageSvc> message_service_;
  MsgStream log_;
};

#endif
