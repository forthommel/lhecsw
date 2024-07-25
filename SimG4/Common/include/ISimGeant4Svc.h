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

#ifndef SimG4_ISimGeant4Svc_h
#define SimG4_ISimGeant4Svc_h

#include <GaudiKernel/IService.h>

// Forward-reference
class G4Event;

/// Interface to the Geant4 service
class ISimGeant4Svc : virtual public IService {
public:
  DeclareInterfaceID(ISimGeant4Svc, 1, 0);
  /// Launch the Geant4 simulation of one event
  virtual StatusCode run(G4Event&, const std::function<bool(const G4Event&)>&) = 0;
  /// Last processed event
  virtual const G4Event* currentEvent() const = 0;
};

#endif
