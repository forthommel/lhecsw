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

#ifndef Generator_BDSIMEventGenerator_h
#define Generator_BDSIMEventGenerator_h

#include <DDG4/Geant4InputAction.h>

#include <BDSIMClass.hh>

class BDSIMObjectOutput;

class BDSIMEventGenerator : public dd4hep::sim::Geant4EventReader {
public:
  explicit BDSIMEventGenerator(const std::string& file_tree_name);
  virtual ~BDSIMEventGenerator() = default;

  using Particles = dd4hep::sim::Geant4InputAction::Particles;
  using Vertices = dd4hep::sim::Geant4InputAction::Vertices;

  EventReaderStatus moveToEvent(int event_number) override;
  inline EventReaderStatus skipEvent() override { return EVENT_READER_OK; }
  EventReaderStatus readParticles(int event_number, Vertices& vertices, Particles& particles) override;
  EventReaderStatus setParameters(std::map<std::string, std::string>& parameters) override;

private:
  const std::string filename_;
  std::unique_ptr<BDSIM> bdsim_;
  BDSIMObjectOutput* output_{nullptr};
};

#endif
