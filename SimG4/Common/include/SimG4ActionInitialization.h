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

#ifndef SimG4_SimG4ActionInitialization_h
#define SimG4_SimG4ActionInitialization_h

#include <G4VUserActionInitialization.hh>

class SimG4ActionInitialization : public G4VUserActionInitialization {
public:
  SimG4ActionInitialization() {}
  virtual ~SimG4ActionInitialization() {}

  void Build() const override {}

private:
};

#endif
