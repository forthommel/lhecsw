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
#include <HepMC3/GenCrossSection.h>
#include <HepMC3/GenEvent.h>
#include <SHERPA/Main/Sherpa.H>

#include "Generator/Common/include/IHepMCProviderTool.h"

using namespace std::string_literals;

class SherpaEventGenerator : public GaudiTool, virtual public IHepMCProviderTool {
public:
  explicit SherpaEventGenerator(const std::string& type, const std::string& name, const IInterface* parent)
      : GaudiTool(type, name, parent),
        sherpa_proc_{this, "process", ""},
        sherpa_path_{this, "path", ""},
        sherpa_path_piece_{this, "pathPiece", ""},
        sherpa_result_dir_{this, "resultDir", ""},
        sherpa_cmds_{this, "commands", {}, "Sherpa commands to be parsed before initialisation"} {}
  virtual ~SherpaEventGenerator() = default;

  inline StatusCode initialize() override {
    if (const auto status = GaudiTool::initialize(); !status.isSuccess())
      return status;

    sherpa_.reset(new SHERPA::Sherpa);
    xsec_.reset(new HepMC3::GenCrossSection);

    std::vector<char*> args;
    for (const auto& cmd : {"./Sherpa"s,
                            "PATH="s + sherpa_path_,
                            "PATH_PIECE="s + sherpa_path_piece_,
                            "RESULT_DIRECTORY="s + sherpa_result_dir_})
      args.emplace_back(const_cast<char*>(cmd.data()));

    sherpa_->InitializeTheRun(args.size(), args.data());
    sherpa_->InitializeTheEventHandler();

    return StatusCode::SUCCESS;
  }
  inline StatusCode finalize() override {
    sherpa_->SummarizeRun();
    sherpa_.release();
    return GaudiTool::finalize();
  }
  inline StatusCode getNextEvent(HepMC3::GenEvent& hepmc_evt) override {
    if (!sherpa_->GenerateOneEvent())
      return Error("Error occurred when running Pythia::next.");
    sherpa_->FillHepMCEvent(hepmc_evt);
    xsec_->set_cross_section(sherpa_->TotalXS(), sherpa_->TotalErr());
    hepmc_evt.set_cross_section(xsec_);
    hepmc_evt.weights().push_back(1.);
    return StatusCode::SUCCESS;
  }

private:
  std::unique_ptr<SHERPA::Sherpa> sherpa_;
  std::shared_ptr<HepMC3::GenCrossSection> xsec_;

  Gaudi::Property<std::string> sherpa_proc_, sherpa_path_, sherpa_path_piece_, sherpa_result_dir_;

  Gaudi::Property<std::vector<std::string> > sherpa_cmds_;  ///< Sherpa commands applied before initialisation
  std::vector<std::string> sherpa_commands_;
};

DECLARE_COMPONENT(SherpaEventGenerator)
