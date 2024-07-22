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

#include "Generator/CepGenInterface/include/CepGenEventGenerator.h"

DECLARE_COMPONENT(CepGenEventGenerator)

CepGenEventGenerator::CepGenEventGenerator(const std::string& type, const std::string& name, const IInterface* parent)
    : GaudiTool(type, name, parent),
      verbosity_{this, "verbosity", static_cast<int>(cepgen::utils::Logger::get().level()), "CepGen verbosity"},
      process_cmds_{this, "process", {}, "CepGen commands to define process"},
      modifiers_cmds_{this, "modifiers", {}, "CepGen modifier->commands to define modifiers chain"},
      outputs_cmds_{this, "outputs", {}, "CepGen output->commands to define outputs chain"} {
  cepgen::utils::Logger::get().setLevel(static_cast<cepgen::utils::Logger::Level>((int)verbosity_));
}

StatusCode CepGenEventGenerator::initialize() {
  if (const auto status = GaudiTool::initialize(); !status.isSuccess())
    return status;

  cepgen_.reset(new cepgen::Generator);
  xsec_.reset(new HepMC3::GenCrossSection);
  cepgen::initialise();

  cepgen::ParametersList plist_proc;
  for (const auto& cmd : process_cmds_)
    plist_proc.feed(cmd);
  info() << "CepGen process parameters:\n" << plist_proc;
  cepgen_->runParameters().setProcess(cepgen::ProcessFactory::get().build(plist_proc));

  for (const auto& mod : modifiers_cmds_) {
    cepgen::ParametersList plist_mod;
    for (const auto& cmd : mod.second)
      plist_mod.feed(cmd);
    cepgen_->runParameters().addModifier(cepgen::EventModifierFactory::get().build(mod.first, plist_mod));
  }

  for (const auto& mod : outputs_cmds_) {
    cepgen::ParametersList plist_out;
    for (const auto& cmd : mod.second)
      plist_out.feed(cmd);
    cepgen_->runParameters().addEventExporter(cepgen::EventExporterFactory::get().build(mod.first, plist_out));
  }

  info() << "CepGen parameters:\n" << cepgen_->runParameters();
  const auto xsec = cepgen_->computeXsection();
  xsec_->set_cross_section(xsec, xsec.uncertainty());
  return StatusCode::SUCCESS;
}

StatusCode CepGenEventGenerator::finalize() {
  cepgen_.release();
  return StatusCode::SUCCESS;
}

StatusCode CepGenEventGenerator::getNextEvent(HepMC3::GenEvent& hepmc_evt) {
  hepmc_evt = HepMC3::CepGenEvent(cepgen_->next());
  hepmc_evt.set_cross_section(xsec_);
  hepmc_evt.weights().push_back(1.);
  return StatusCode::SUCCESS;
}
