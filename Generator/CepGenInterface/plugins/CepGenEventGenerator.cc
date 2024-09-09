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

// CepGen
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

#include "Generator/CepGenInterface/include/ParametersListConverter.h"
#include "Generator/Common/include/IHepMCProviderTool.h"

class CepGenEventGenerator : public art::EDProducer {
public:
  explicit CepGenEventGenerator(const fhicl::ParameterSet& iConfig)
      : art::EDProducer(iConfig),
        verbosity_{iConfig.get<int>("verbosity")},
        process_str_{iConfig.get<std::string>("process")},
        extra_process_str_{iConfig.get<std::vector<std::string> >("extraProcessCommands")},
        modifiers_str_{iConfig.get<std::string>("modifiers")},
        outputs_str_{iConfig.get<std::string>("outputs")} {
    produces<HepMC3::GenEvent>("genParticles");
    cepgen::utils::Logger::get().setLevel(static_cast<cepgen::utils::Logger::Level>(verbosity_.value()));
    cepgen_.reset(new cepgen::Generator);
    xsec_.reset(new HepMC3::GenCrossSection);

    auto plist_proc = static_cast<cepgen::ParametersList>(cepgen::ParametersListConverter(process_str_));
    for (const auto& cmd : extra_process_str_)
      plist_proc.feed(cmd);
    info() << "CepGen process parameters:\n" << plist_proc;
    cepgen_->runParameters().setProcess(cepgen::ProcessFactory::get().build(plist_proc));

    const auto plist_mods = static_cast<cepgen::ParametersList>(cepgen::ParametersListConverter(modifiers_str_));
    for (const auto& mod :
         plist_mods.keysOf<cepgen::ParametersList>())  // browse the sequence of event modifier modules
      cepgen_->runParameters().addModifier(
          cepgen::EventModifierFactory::get().build(mod, plist_mods.get<cepgen::ParametersList>(mod)));

    const auto plist_outputs = static_cast<cepgen::ParametersList>(cepgen::ParametersListConverter(outputs_str_));
    for (const auto& mod : plist_outputs.keysOf<cepgen::ParametersList>())  // browse the sequence of output modules
      cepgen_->runParameters().addEventExporter(
          cepgen::EventExporterFactory::get().build(mod, plist_outputs.get<cepgen::ParametersList>(mod)));

    info() << "CepGen parameters:\n" << cepgen_->runParameters();
    const auto xsec = cepgen_->computeXsection();
    xsec_->set_cross_section(xsec, xsec.uncertainty());
  }
  virtual ~CepGenEventGenerator() = default;
  //"verbosity", static_cast<int>(cepgen::utils::Logger::get().level()), "CepGen verbosity"},
  //"process", "{}", "CepGen commands to define process"},
  //"extraProcessCommands", {}, "Additional CepGen string-type commands to define process"},
  //"modifiers", "{}", "Sequential list of CepGen modifier modules defining the chain"},
  //"outputs", "{}", "Sequential list of CepGen output modules defining the chain"} {

  void produce(art::Event& iEvent) override {
    std::unique_ptr<HepMC3::GenEvent> evt;
    evt.reset(new HepMC3::CepGenEvent(cepgen_->next()));
    evt->set_cross_section(xsec_);
    evt->weights().push_back(1.);
    iEvent.put(std::move(evt), "genParticles");
  }

private:
  const int verbosity_;                               ///< CepGen module verbosity
  const std::string process_str_;                     ///< JSON dump of CepGen commands to define process
  const std::vector<std::string> extra_process_str_;  ///< Extra string CepGen commands to define process
  const std::string modifiers_str_;
  const std::string outputs_str_;

  std::unique_ptr<cepgen::Generator> cepgen_;
  std::shared_ptr<HepMC3::GenCrossSection> xsec_;
};

DEFINE_ART_MODULE(CepGenEventGenerator)
