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

#include <Pythia8/Pythia.h>
#include <Pythia8Plugins/HepMC3.h>

#include "Generator/Pythia8Interface/include/Pythia8EventGenerator.h"

DECLARE_COMPONENT(Pythia8EventGenerator)

Pythia8EventGenerator::Pythia8EventGenerator(const std::string& type, const std::string& name, const IInterface* parent)
    : GaudiTool(type, name, parent),
      cmds_file_{this, "fileCommands", "", "Pythia8 commands file to be parsed"},
      cmds_preinit_{this, "preInitCommands", {}, "Pythia8 commands to be parsed before initialisation"},
      cmds_postinit_{this, "postInitCommands", {}, "Pythia8 commands to be parsed after initialisation"} {}

StatusCode Pythia8EventGenerator::initialize() {
  if (const auto status = GaudiTool::initialize(); !status.isSuccess())
    return status;

  pythia_.reset(new Pythia8::Pythia);
  hepmc_.reset(new HepMC3::Pythia8ToHepMC3);

  for (const auto& cmd : cmds_preinit_)
    if (!pythia_->readString(cmd))
      return Error("Failed to parse input Pythia command '" + cmd + "'.");

  if (!cmds_file_.empty() && !pythia_->readFile(cmds_file_))
    return Error("Failed to load input Pythia commands file '" + cmds_file_ + "'.");

  if (!pythia_->init())  // Pythia initialisation
    return Error("Failed to load parse configuration commands.");

  for (const auto& cmd : cmds_postinit_)
    if (!pythia_->readString(cmd))
      return Error("Failed to parse input Pythia command '" + cmd + "'.");

  return StatusCode::SUCCESS;
}

StatusCode Pythia8EventGenerator::finalize() {
  pythia_.release();
  hepmc_.release();
  return GaudiTool::finalize();
}

StatusCode Pythia8EventGenerator::getNextEvent(HepMC3::GenEvent& hepmc_evt) {
  if (!pythia_->next())
    return Error("Error occurred when running Pythia::next.");
  if (!hepmc_->fill_next_event(*pythia_, hepmc_evt))
    return Error("Failed to convert Pythia 8 event into an HepMC3 event.");

  return StatusCode::SUCCESS;
}
