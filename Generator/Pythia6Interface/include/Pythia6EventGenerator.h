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

#ifndef Generator_Pythia6Interface_Pythia6EventGenerator_h
#define Generator_Pythia6Interface_Pythia6EventGenerator_h

#include <DDG4/Geant4InputAction.h>
#include <HepMC3/HEPEVT_Wrapper_Runtime.h>

#include "Generator/Common/include/HepMC3EventConverter.h"

class Pythia6EventGenerator : public dd4hep::sim::Geant4EventReader {
public:
  explicit Pythia6EventGenerator(const std::string&);
  virtual ~Pythia6EventGenerator() = default;

  using Particles = dd4hep::sim::Geant4InputAction::Particles;
  using Vertices = dd4hep::sim::Geant4InputAction::Vertices;

  EventReaderStatus moveToEvent(int event_number) override;
  inline EventReaderStatus skipEvent() override { return EVENT_READER_OK; }
  EventReaderStatus readParticles(int event_number, Vertices& vertices, Particles& particles) override;
  EventReaderStatus setParameters(std::map<std::string, std::string>& parameters) override;

private:
  const std::string filename_;
  HepMC3::HEPEVT_Wrapper_Runtime hepevt_wrapper_;
  HepMC3EventConverter hepmc_converter_;
};

#endif
