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

#ifndef Generator_Pythia8Interface_Pythia8EventGenerator_h
#define Generator_Pythia8Interface_Pythia8EventGenerator_h

#include <DDG4/Geant4InputAction.h>
#include <Pythia8/Pythia.h>
#include <Pythia8Plugins/HepMC3.h>

class Pythia8EventGenerator : public dd4hep::sim::Geant4EventReader {
public:
  explicit Pythia8EventGenerator(const std::string&);
  virtual ~Pythia8EventGenerator() = default;

  using Particles = dd4hep::sim::Geant4InputAction::Particles;
  using Vertices = dd4hep::sim::Geant4InputAction::Vertices;

  EventReaderStatus moveToEvent(int event_number) override;
  inline EventReaderStatus skipEvent() override { return EVENT_READER_OK; }
  EventReaderStatus readParticles(int event_number, Vertices& vertices, Particles& particles) override;
  EventReaderStatus setParameters(std::map<std::string, std::string>& parameters) override;

private:
  const std::unique_ptr<Pythia8::Pythia> pythia_;
  const std::unique_ptr<HepMC3::Pythia8ToHepMC3> hepmc_;
  const std::string filename_;
};

#endif
