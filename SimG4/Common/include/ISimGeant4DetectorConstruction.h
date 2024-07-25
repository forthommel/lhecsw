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

#ifndef SimG4_ISimGeant4DetectorConstruction_h
#define SimG4_ISimGeant4DetectorConstruction_h

#include <GaudiKernel/IAlgTool.h>

// Forward-reference
class G4VUserDetectorConstruction;

/// Interface to the DD4hep-to-Geant4 detector construction tool
class ISimGeant4DetectorConstruction : public IAlgTool {
public:
  DeclareInterfaceID(ISimGeant4DetectorConstruction, 1, 0);
  /// Geometry initialisation hook
  virtual G4VUserDetectorConstruction* detectorConstruction() = 0;
};

#endif
