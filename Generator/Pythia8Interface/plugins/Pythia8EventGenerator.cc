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

// art
#include <art/Framework/Core/EDProducer.h>

// Pythia
#include <Pythia8/Pythia.h>
#include <Pythia8Plugins/HepMC3.h>

#include "Generator/Common/include/IHepMCProviderTool.h"

class Pythia8EventGenerator : public art::EDProducer {
public:
  explicit Pythia8EventGenerator(const fhicl::ParameterSet& iConfig)
      : art::EDProducer(iConfig),
        cmds_file_{iConfig.get<std::string>("fileCommands")},
        cmds_preinit_{iConfig.get<std::vector<std::string> >("preInitCommands")},
        cmds_postinit_{iConfig.get<std::vector<std::string> >("postInitCommands")},
        pythia_(new Pythia8::Pythia),
        hepmc_(new HepMC3::Pythia8ToHepMC3) {}
  virtual ~Pythia8EventGenerator() = default;
  // "fileCommands", "Pythia8 commands file to be parsed"
  // "preInitCommands", "Pythia8 commands to be parsed before initialisation"
  // "postInitCommands", "Pythia8 commands to be parsed after initialisation"

  void beginRun(const art::Event&) override {
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
  }
  void produce(art::Event& iEvent) override {
    if (!pythia_->next())
      return Error("Error occurred when running Pythia::next.");
    std::unique_ptr<HepMC3::GenEvent> evt;
    if (!hepmc_->fill_next_event(*pythia_, evt.get()))
      return Error("Failed to convert Pythia 8 event into an HepMC3 event.");
    iEvent.put(std::move(evt), "genParticles");
  }

private:
  const std::string cmds_file_;                   ///< Pythia8 commands file to be applied
  const std::vector<std::string> cmds_preinit_;   ///< Pythia8 commands applied before initialisation
  const std::vector<std::string> cmds_postinit_;  ///< Pythia8 commands applied after initialisation

  std::unique_ptr<Pythia8::Pythia> pythia_;
  std::unique_ptr<HepMC3::Pythia8ToHepMC3> hepmc_;
};

DECLARE_COMPONENT(Pythia8EventGenerator)
