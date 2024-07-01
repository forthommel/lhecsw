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
#include <CepGen/Modules/EventExporterFactory.h>
#include <CepGen/Modules/EventModifierFactory.h>
#include <CepGen/Modules/ProcessFactory.h>
#include <CepGen/Process/Process.h>
#include <CepGenAddOns/HepMC3Wrapper/HepMC3EventInterface.h>
#include <DDG4/Factories.h>
#include <DDG4/Geant4InputAction.h>

#include "Generator/CepGenInterface/include/CepGenEventGenerator.h"
#include "Generator/Common/include/HepMC3EventConverter.h"

using EventReaderStatus = dd4hep::sim::Geant4EventReader::EventReaderStatus;

CepGenEventGenerator::CepGenEventGenerator(const std::string& filename)
    : dd4hep::sim::Geant4EventReader(filename), cepgen_(new cepgen::Generator), xsec_(new HepMC3::GenCrossSection) {}

EventReaderStatus CepGenEventGenerator::moveToEvent(int) { return EVENT_READER_OK; }

EventReaderStatus CepGenEventGenerator::readParticles(int, Vertices& vertices, Particles& particles) {
  auto* evt = new HepMC3::CepGenEvent(cepgen_->next());
  evt->set_cross_section(xsec_);
  evt->weights().push_back(1.);
  if (HepMC3EventConverter::convert(evt, vertices, particles))
    return EVENT_READER_OK;
  return EVENT_READER_IO_ERROR;
}

EventReaderStatus CepGenEventGenerator::setParameters(std::map<std::string, std::string>& parameters) {
  int verb = 0;
  std::list<std::string> process_commands;
  std::vector<std::list<std::string> > modif_commands, output_commands;
  _getParameterValue(parameters, "Verbosity", verb, verb);
  _getParameterValue(parameters, "Process", process_commands, process_commands);
  _getParameterValue(parameters, "Modifier", modif_commands, modif_commands);
  _getParameterValue(parameters, "Output", output_commands, output_commands);

  cepgen::utils::Logger::get().setLevel(static_cast<cepgen::utils::Logger::Level>(verb));

  cepgen::ParametersList plist_proc;
  for (const auto& cmd : process_commands)
    plist_proc.feed(cmd);
  cepgen_->runParameters().setProcess(cepgen::ProcessFactory::get().build(plist_proc));

  for (const auto& modif : modif_commands) {
    cepgen::ParametersList plist_mod;
    for (const auto& cmd : modif)
      plist_mod.feed(cmd);
    cepgen_->runParameters().addModifier(cepgen::EventModifierFactory::get().build(plist_mod));
  }

  for (const auto& output : output_commands) {
    cepgen::ParametersList plist_mod;
    for (const auto& cmd : output)
      plist_mod.feed(cmd);
    cepgen_->runParameters().addEventExporter(cepgen::EventExporterFactory::get().build(plist_mod));
  }

  const auto xsec = cepgen_->computeXsection();
  xsec_->set_cross_section(xsec, xsec.uncertainty());

  return EVENT_READER_OK;
}

DECLARE_GEANT4_EVENT_READER(CepGenEventGenerator)  // add to the factory
