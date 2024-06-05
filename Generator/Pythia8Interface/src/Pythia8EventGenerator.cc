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

EventReaderStatus Pythia8EventGenerator::moveToEvent(int) { return EVENT_READER_OK; }

EventReaderStatus Pythia8EventGenerator::readParticles(int, Vertices& vertices, Particles& particles) {
  vertices.clear();
  particles.clear();
  if (!pythia_->next()) {
    dd4hep::printout(
        dd4hep::DEBUG, "Pythia8EventGenerator::readParticles", "Error occurred when running Pythia::next.");
    return EVENT_READER_ERROR;
  }
  Vertex *vtx_beam1{nullptr}, *vtx_beam2{nullptr};
  for (int i = 0; i < pythia_->event.size(); ++i) {
    const auto& pypart = pythia_->event.at(i);
    auto part = dd4hep::sim::Geant4ParticleHandle(new Particle(i));
    part->status = pypart.status();
    part->genStatus = pypart.status();
    part->pdgID = pypart.id();
    part->charge = pypart.charge();
    part->colorFlow[0] = pypart.col();
    part->colorFlow[1] = pypart.acol();
    part->vsx = part->vex = pypart.xProd() * dd4hep::cm;
    part->vsy = part->vey = pypart.yProd() * dd4hep::cm;
    part->vsz = part->vez = pypart.zProd() * dd4hep::cm;
    part->time = part->properTime = pypart.tProd() * dd4hep::s;
    part->psx = part->pex = pypart.px() * dd4hep::GeV;
    part->psy = part->pey = pypart.py() * dd4hep::GeV;
    part->psz = part->pez = pypart.pz() * dd4hep::GeV;
    part->mass = pypart.mCalc() * dd4hep::GeV;
    dd4hep::printout(dd4hep::INFO,
                     "Pythia8EventGenerator::readParticles",
                     "Added particle #%d with PDG id=%d, charge=%de, status=%d/%d, momentum=(%g, %g, %g)",
                     part->id,
                     part->pdgID,
                     part->charge,
                     part->status,
                     part->genStatus,
                     part->psx,
                     part->psy,
                     part->psz);
    particles.emplace_back(part);
    const auto moth1 = pypart.mother1(), moth2 = pypart.mother2();
    Vertex* vtx{nullptr};
    if (moth1 == 0 && moth2 == 0) {  // primary (beam) particles
      if (i == 0) {
        vtx_beam1 = vertices.emplace_back();
        vtx = vtx_beam1;
      } else if (i == 1) {
        vtx_beam2 = vertices.emplace_back();
        vtx = vtx_beam2;
      } else
        dd4hep::except(
            "Pythia8EventGenerator::readParticles", "Invalid parentage for non-beam particle (%d, %d).", moth1, moth2);
    }
  }
  return EVENT_READER_OK;
}

EventReaderStatus Pythia8EventGenerator::setParameters(std::map<std::string, std::string>& parameters) {
  std::list<std::string> pre_commands, post_commands;
  _getParameterValue(parameters, "Commands", pre_commands, pre_commands);
  _getParameterValue(parameters, "PostCommands", post_commands, post_commands);

  for (const auto& cmd : pre_commands)  // steer the pre-initialisation parameters
    pythia_->readString(cmd);

  if (!pythia_->init()) {  // Pythia initialisation
    dd4hep::except("Pythia8EventGenerator::Pythia8EventGenerator", "Failed to load parse configuration commands.");
    return EVENT_READER_ERROR;
  }
  for (const auto& cmd : post_commands)  // steer the post-initialisation parameters
    pythia_->readString(cmd);

  return EVENT_READER_OK;
}

DECLARE_GEANT4_EVENT_READER(Pythia8EventGenerator)  // add to the factory
