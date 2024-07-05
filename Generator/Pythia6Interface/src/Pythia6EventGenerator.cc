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
#include <HepMC3/HEPEVT_Helpers.h>

#include "Common/include/StringUtils.h"
#include "Generator/Pythia6Interface/include/Pythia6EventGenerator.h"
#include "Generator/Pythia6Interface/include/Pythia6Interface.h"

using EventReaderStatus = dd4hep::sim::Geant4EventReader::EventReaderStatus;
using PropertyMask = dd4hep::detail::ReferenceBitMask<int>;

static constexpr size_t max_particles_pythia = 4000;
extern "C" {
extern HepMC3::HEPEVT_Templated<max_particles_pythia, double> hepevt_;
}

Pythia6EventGenerator::Pythia6EventGenerator(const std::string& filename)
    : dd4hep::sim::Geant4EventReader(filename), filename_(filename) {
  hepevt_wrapper_.set_hepevt_address((char*)&hepevt_);
  hepevt_wrapper_.set_max_number_entries(max_particles_pythia);
  hepevt_wrapper_.zero_everything();
  hepmc_converter_.particle_properties_getter = [=](int pdgid) {
    HepMC3EventConverter::ParticleProperties props;
    props.charge = pythia6::pychge(pdgid);
    return props;
  };
}

EventReaderStatus Pythia6EventGenerator::moveToEvent(int) { return EVENT_READER_OK; }

EventReaderStatus Pythia6EventGenerator::readParticles(int, Vertices& vertices, Particles& particles) {
  pythia6::pyexec();
  dd4hep::printout(dd4hep::DEBUG, __PRETTY_FUNCTION__, "Finished running the PYEXEC event generation subroutine.");
  if (pythia6::pypars().msti.at(53 - 1) == 1)
    dd4hep::except(__PRETTY_FUNCTION__, "No processes with non-vanishing cross sections were found in the PYINIT call");
  dd4hep::printout(dd4hep::DEBUG,
                   __PRETTY_FUNCTION__,
                   "Generated cross section for the process: %g mb.",
                   pythia6::pypars().pari.at(1 - 1));

  pythia6::pyhepc(1);
  dd4hep::printout(dd4hep::DEBUG, __PRETTY_FUNCTION__, "Finished running the PYHEPC conversion subroutine.");
  {
    std::ostringstream hepevt_os;
    hepevt_wrapper_.print_hepevt(hepevt_os);
    dd4hep::printout(dd4hep::DEBUG, __PRETTY_FUNCTION__, "HEPEVT event content:\n%s", hepevt_os.str().c_str());
  }
  HepMC3::GenEvent hepmc_evt(HepMC3::Units::GEV, HepMC3::Units::MM);
  if (!hepevt_wrapper_.HEPEVT_to_GenEvent(&hepmc_evt))
    dd4hep::except(__PRETTY_FUNCTION__, "Failed to convert Pythia 8 event into an HepMC3 event.");
  dd4hep::printout(dd4hep::DEBUG, __PRETTY_FUNCTION__, "HepMC3 conversion done.");

  if (hepmc_converter_.convert(hepmc_evt, vertices, particles))
    return EVENT_READER_OK;
  return EVENT_READER_IO_ERROR;
}

EventReaderStatus Pythia6EventGenerator::setParameters(std::map<std::string, std::string>& parameters) {
  std::list<std::string> pre_commands, post_commands;
  _getParameterValue(parameters, "Commands", pre_commands, pre_commands);
  _getParameterValue(parameters, "PostCommands", post_commands, post_commands);

  if (!filename_.empty())  // read a commands input file
    if (const auto tokens = utils::split(filename_, '|', true); tokens.size() > 1) {
      std::ifstream file(tokens.at(1));
      std::string line;
      while (std::getline(file, line))
        pythia6::pygive(line);
    }
  for (const auto& cmd : pre_commands)  // steer the pre-initialisation parameters
    pythia6::pygive(cmd);
  pythia6::pyinit("3MOM", "p", "e-", 0. /* will be computed automatically from beams momenta */);
  for (const auto& cmd : post_commands)  // steer the post-initialisation parameters
    pythia6::pygive(cmd);

  return EVENT_READER_OK;
}

DECLARE_GEANT4_EVENT_READER(Pythia6EventGenerator)  // add to the factory
