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

#include <DDG4/Factories.h>
#include <DDG4/Geant4InputAction.h>
#include <TFile.h>
#include <TTree.h>

#include "Generator/Pythia8Interfaceinclude/Pythia8EventGenerator.h"

using EventReaderStatus = dd4hep::sim::Geant4EventReader::EventReaderStatus;
using PropertyMask = dd4hep::detail::ReferenceBitMask<int>;

Pythia8EventGenerator::Pythia8EventGenerator(const std::string& filename)
    : dd4hep::sim::Geant4EventReader(filename), pythia_(new Pythia8::Pythia), filename_(filename) {
  m_directAccess = true;
}

EventReaderStatus Pythia8EventGenerator::moveToEvent(int event_number) { return EVENT_READER_OK; }

EventReaderStatus Pythia8EventGenerator::readParticles(int, Vertices& vertices, Particles& particles) {
  vertices.clear();
  particles.clear();
  return EVENT_READER_OK;
}

EventReaderStatus Pythia8EventGenerator::setParameters(std::map<std::string, std::string>& parameters) {
  return EVENT_READER_OK;
}

DECLARE_GEANT4_EVENT_READER(Pythia8EventGenerator)  // add to the factory
