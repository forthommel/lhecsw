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

#include <CepGen/Core/Exception.h>
#include <CepGen/Core/ParametersList.h>
#include <CepGen/Core/RunParameters.h>
#include <CepGen/EventFilter/EventExporter.h>
#include <CepGen/EventFilter/EventModifier.h>
#include <CepGen/Generator.h>
#include <CepGen/Modules/EventExporterFactory.h>
#include <CepGen/Modules/EventModifierFactory.h>
#include <CepGen/Modules/ProcessFactory.h>
#include <CepGen/Physics/PDG.h>
#include <CepGen/Process/Process.h>
#include <CepGenAddOns/HepMC3Wrapper/HepMC3EventInterface.h>
#include <GaudiAlg/GaudiTool.h>

#include "Generator/Common/include/IHepMCProviderTool.h"

class CepGenEventGenerator : public GaudiTool, virtual public IHepMCProviderTool {
public:
  explicit CepGenEventGenerator(const std::string& type, const std::string& name, const IInterface* parent)
      : GaudiTool(type, name, parent),
        verbosity_{this, "verbosity", static_cast<int>(cepgen::utils::Logger::get().level()), "CepGen verbosity"},
        process_cmds_{this, "process", {}, "CepGen commands to define process"},
        modifiers_seq_{this, "modifiersSequence", {}, "Sequential list of CepGen modifier modules defining the chain"},
        outputs_seq_{this, "outputsSequence", {}, "Sequential list of CepGen output modules defining the chain"},
        modifiers_cmds_{this, "modifiers", {}, "CepGen modifier->commands to define modifiers chain"},
        outputs_cmds_{this, "outputs", {}, "CepGen output->commands to define outputs chain"} {
    cepgen::utils::Logger::get().setLevel(static_cast<cepgen::utils::Logger::Level>(verbosity_.value()));
  }
  virtual ~CepGenEventGenerator() = default;

  StatusCode initialize() override;
  inline StatusCode finalize() override {
    cepgen_.release();
    return StatusCode::SUCCESS;
  }
  inline StatusCode getNextEvent(HepMC3::GenEvent& hepmc_evt) override {
    hepmc_evt = HepMC3::CepGenEvent(cepgen_->next());
    hepmc_evt.set_cross_section(xsec_);
    hepmc_evt.weights().push_back(1.);
    return StatusCode::SUCCESS;
  }

private:
  std::unique_ptr<cepgen::Generator> cepgen_;
  std::shared_ptr<HepMC3::GenCrossSection> xsec_;

  Gaudi::Property<int> verbosity_;                           ///< CepGen module verbosity
  Gaudi::Property<std::vector<std::string> > process_cmds_;  ///< CepGen commands to define process
  Gaudi::Property<std::vector<std::string> > modifiers_seq_, outputs_seq_;
  Gaudi::Property<std::map<std::string, std::vector<std::string> > > modifiers_cmds_;  ///< Modifiers chain
  Gaudi::Property<std::map<std::string, std::vector<std::string> > > outputs_cmds_;    ///< Outputs chain
};

DECLARE_COMPONENT(CepGenEventGenerator)

StatusCode CepGenEventGenerator::initialize() {
  if (const auto status = GaudiTool::initialize(); !status.isSuccess())
    return status;

  cepgen_.reset(new cepgen::Generator);
  xsec_.reset(new HepMC3::GenCrossSection);

  cepgen::ParametersList plist_proc;
  for (const auto& cmd : process_cmds_)
    plist_proc.feed(cmd);
  info() << "CepGen process parameters:\n" << plist_proc;
  cepgen_->runParameters().setProcess(cepgen::ProcessFactory::get().build(plist_proc));

  for (const auto& mod : modifiers_seq_)          // browse the sequential list of event modifier modules requested
    if (modifiers_cmds_.value().count(mod) == 0)  // use default parameters for the module
      cepgen_->runParameters().addModifier(cepgen::EventModifierFactory::get().build(mod));
    else {  // parse the list of parameters fed by the user
      cepgen::ParametersList plist_mod;
      for (const auto& cmd : modifiers_cmds_.value().at(mod))
        plist_mod.feed(cmd);
      cepgen_->runParameters().addModifier(cepgen::EventModifierFactory::get().build(mod, plist_mod));
    }

  for (const auto& mod : outputs_seq_)          // browse the sequential list of output modules requested
    if (outputs_cmds_.value().count(mod) == 0)  // use default parameters for the module
      cepgen_->runParameters().addEventExporter(cepgen::EventExporterFactory::get().build(mod));
    else {  // parse the list of parameters fed by the user
      cepgen::ParametersList plist_out;
      for (const auto& cmd : outputs_cmds_.value().at(mod))
        plist_out.feed(cmd);
      cepgen_->runParameters().addEventExporter(cepgen::EventExporterFactory::get().build(mod, plist_out));
    }

  info() << "CepGen parameters:\n" << cepgen_->runParameters();
  const auto xsec = cepgen_->computeXsection();
  xsec_->set_cross_section(xsec, xsec.uncertainty());
  return StatusCode::SUCCESS;
}
