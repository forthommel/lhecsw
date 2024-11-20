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

#include <GaudiAlg/GaudiTool.h>
#include <HepMC3/HEPEVT_Helpers.h>
#include <HepMC3/HEPEVT_Wrapper_Runtime.h>

#include "FWCore/Utils/include/StringUtils.h"
#include "Generator/Common/include/IHepMCProviderTool.h"
#include "Generator/Pythia6Interface/include/Pythia6Interface.h"

static constexpr size_t max_particles_pythia = 4000;
extern "C" {
extern HepMC3::HEPEVT_Templated<max_particles_pythia, double> hepevt_;
}

class Pythia6EventGenerator : public GaudiTool, virtual public IHepMCProviderTool {
public:
  explicit Pythia6EventGenerator(const std::string& type, const std::string& name, const IInterface* parent)
      : GaudiTool(type, name, parent),
        cmds_preinit_{this, "preInitCommands", {}, "Pythia6 commands to be parsed before initialisation"},
        cmds_postinit_{this, "postInitCommands", {}, "Pythia6 commands to be parsed after initialisation"} {
    hepevt_wrapper_.set_hepevt_address((char*)&hepevt_);
    hepevt_wrapper_.set_max_number_entries(max_particles_pythia);
    hepevt_wrapper_.zero_everything();
  }
  virtual ~Pythia6EventGenerator() = default;

  inline StatusCode initialize() override {
    if (const auto ret = GaudiTool::initialize(); !ret.isSuccess())
      return ret;
    for (const auto& cmd : cmds_preinit_)  // steer the pre-initialisation parameters
      pythia6::pygive(cmd);
    pythia6::pyinit("3MOM", "p", "e-", 0. /* will be computed automatically from beams momenta */);
    for (const auto& cmd : cmds_postinit_)  // steer the post-initialisation parameters
      pythia6::pygive(cmd);
    return StatusCode::SUCCESS;
  }
  inline StatusCode finalize() override { return GaudiTool::finalize(); }
  inline StatusCode getNextEvent(HepMC3::GenEvent& hepmc_evt) override {
    pythia6::pyexec();
    debug() << "Finished running the PYEXEC event generation subroutine.";
    if (pythia6::pypars().msti.at(53 - 1) == 1)
      return Error("No processes with non-vanishing cross sections were found in the PYINIT call");
    debug() << "Generated cross section for the process: " << pythia6::pypars().pari.at(1 - 1) << " mb.";

    pythia6::pyhepc(1);
    debug() << "Finished running the PYHEPC conversion subroutine.";
    {
      std::ostringstream hepevt_os;
      hepevt_wrapper_.print_hepevt(hepevt_os);
      debug() << "HEPEVT event content:\n" << hepevt_os.str();
    }
    if (!hepevt_wrapper_.HEPEVT_to_GenEvent(&hepmc_evt))
      return Error("Failed to convert Pythia event into an HepMC3 event.");
    debug() << "HepMC3 conversion done.";
    return StatusCode::SUCCESS;
  }

private:
  HepMC3::HEPEVT_Wrapper_Runtime hepevt_wrapper_;
  Gaudi::Property<std::vector<std::string> > cmds_preinit_, cmds_postinit_;
};

DECLARE_COMPONENT(Pythia6EventGenerator)
