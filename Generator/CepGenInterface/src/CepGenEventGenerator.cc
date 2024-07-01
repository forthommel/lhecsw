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

#include <CepGenAddOns/HepMC3Wrapper/HepMC3EventInterface.h>
#include <DDG4/Factories.h>
#include <DDG4/Geant4InputAction.h>

#include "Generator/CepGenInterface/include/CepGenEventGenerator.h"

using EventReaderStatus = dd4hep::sim::Geant4EventReader::EventReaderStatus;

CepGenEventGenerator::CepGenEventGenerator(const std::string& filename)
    : dd4hep::sim::Geant4EventReader(filename), cepgen_(new cepgen::Generator) {}

EventReaderStatus CepGenEventGenerator::moveToEvent(int) { return EVENT_READER_OK; }

EventReaderStatus CepGenEventGenerator::readParticles(int, Vertices& vertices, Particles& particles) {
  vertices.clear();
  particles.clear();
  return EVENT_READER_OK;
}

EventReaderStatus CepGenEventGenerator::setParameters(std::map<std::string, std::string>& parameters) {
  std::list<std::string> commands;
  _getParameterValue(parameters, "Commands", commands, commands);

  return EVENT_READER_OK;
}

DECLARE_GEANT4_EVENT_READER(CepGenEventGenerator)  // add to the factory
