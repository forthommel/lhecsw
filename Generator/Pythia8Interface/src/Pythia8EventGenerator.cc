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

#include "Common/include/StringUtils.h"
#include "Generator/Pythia8Interface/include/Pythia8EventGenerator.h"

using EventReaderStatus = dd4hep::sim::Geant4EventReader::EventReaderStatus;
using PropertyMask = dd4hep::detail::ReferenceBitMask<int>;

Pythia8EventGenerator::Pythia8EventGenerator(const std::string& filename)
    : dd4hep::sim::Geant4EventReader(filename),
      pythia_(new Pythia8::Pythia),
      hepmc_(new HepMC3::Pythia8ToHepMC3),
      filename_(filename) {
  hepmc_converter_.particle_properties_getter = [this](int pdgid) {
    HepMC3EventConverter::ParticleProperties props;
    props.charge = pythia_->particleData.charge(pdgid);
    return props;
  };
}

EventReaderStatus Pythia8EventGenerator::moveToEvent(int) { return EVENT_READER_OK; }

EventReaderStatus Pythia8EventGenerator::readParticles(int, Vertices& vertices, Particles& particles) {
  if (!pythia_->next()) {
    dd4hep::printout(
        dd4hep::DEBUG, "Pythia8EventGenerator::readParticles", "Error occurred when running Pythia::next.");
    return EVENT_READER_ERROR;
  }
  HepMC3::GenEvent hepmc_evt(HepMC3::Units::GEV, HepMC3::Units::MM);
  if (!hepmc_->fill_next_event(*pythia_, hepmc_evt))
    dd4hep::except("Pythia8EventGenerator::readParticles", "Failed to convert Pythia 8 event into an HepMC3 event.");

  if (hepmc_converter_.convert(hepmc_evt, vertices, particles))
    return EVENT_READER_OK;
  return EVENT_READER_IO_ERROR;
}

EventReaderStatus Pythia8EventGenerator::setParameters(std::map<std::string, std::string>& parameters) {
  std::list<std::string> pre_commands, post_commands;
  _getParameterValue(parameters, "Commands", pre_commands, pre_commands);
  _getParameterValue(parameters, "PostCommands", post_commands, post_commands);

  if (!filename_.empty()) {
    const auto tokens = utils::split(filename_, '|', true);
    if (tokens.size() > 1 && !pythia_->readFile(tokens.at(1))) {
      dd4hep::except("Pythia8EventGenerator::setParameters",
                     "Failed to load input Pythia commands file '" + tokens.at(1) + "'.");
      return EVENT_READER_ERROR;
    }
  }
  for (const auto& cmd : pre_commands)  // steer the pre-initialisation parameters
    if (!pythia_->readString(cmd)) {
      dd4hep::except("Pythia8EventGenerator::setParameters", "Failed to parse input Pythia command '" + cmd + "'.");
      return EVENT_READER_ERROR;
    }

  if (!pythia_->init()) {  // Pythia initialisation
    dd4hep::except("Pythia8EventGenerator::setParameters", "Failed to load parse configuration commands.");
    return EVENT_READER_ERROR;
  }
  for (const auto& cmd : post_commands)  // steer the post-initialisation parameters
    if (!pythia_->readString(cmd)) {
      dd4hep::except("Pythia8EventGenerator::setParameters", "Failed to parse input Pythia command '" + cmd + "'.");
      return EVENT_READER_ERROR;
    }

  return EVENT_READER_OK;
}

DECLARE_GEANT4_EVENT_READER(Pythia8EventGenerator)  // add to the factory
