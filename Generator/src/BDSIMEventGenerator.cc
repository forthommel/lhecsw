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

#include <BDSException.hh>

#include "Generator/include/BDSIMEventGenerator.h"
#include "Generator/include/BDSIMObjectOutput.h"

using namespace std::string_literals;
using EventReaderStatus = dd4hep::sim::Geant4EventReader::EventReaderStatus;
using PropertyMask = dd4hep::detail::ReferenceBitMask<int>;

BDSIMEventGenerator::BDSIMEventGenerator(const std::string& filename) : dd4hep::sim::Geant4EventReader(filename) {}

EventReaderStatus BDSIMEventGenerator::moveToEvent(int) { return EVENT_READER_OK; }

EventReaderStatus BDSIMEventGenerator::readParticles(int, Vertices& vertices, Particles& particles) {
  if (!output_)
    return EVENT_READER_IO_ERROR;
  vertices.clear();
  particles.clear();
  try {
    bdsim_->BeamOn(1);
  } catch (const BDSException& except) {
    dd4hep::except("BDSIMEventGenerator::setParameters", "BDSIM exception: %s", except.what());
  } catch (const std::exception& except) {
    dd4hep::except("BDSIMEventGenerator::setParameters", "Generic exception: %s", except.what());
  }
  return EVENT_READER_OK;
}

EventReaderStatus BDSIMEventGenerator::setParameters(std::map<std::string, std::string>& parameters) {
  const auto args = std::vector<std::string>{};
  std::vector<char*> argv;
  std::transform(args.begin(), args.end(), std::back_inserter(argv), [](const std::string& str) -> char* {
    char* out = new char[str.size() + 1];
    std::strcpy(out, str.data());
    return out;
  });
  try {
    bdsim_.reset(new BDSIM(argv.size(), argv.data()));
    if (!bdsim_->Initialised()) {
      if (bdsim_->InitialisationResult() == 1)
        dd4hep::except("BDSIMEventGenerator::setParameters", "Initialisation failed.");
      return EVENT_READER_ERROR;
    }
    //bdsim_->SetOutput(new BDSIMObjectOutput);  //FIXME need to adapt BDSIM API
    //output_ = bdsim_->GetOutput();             //FIXME need to adapt BDSIM API
  } catch (const BDSException& except) {
    dd4hep::except("BDSIMEventGenerator::setParameters", "BDSIM exception: %s", except.what());
  } catch (const std::exception& except) {
    dd4hep::except("BDSIMEventGenerator::setParameters", "Generic exception: %s", except.what());
  }

  /*_getParameterValue(parameters, "TreeName", tree_name_, "Event"s);
  _getParameterValue(parameters, "ScorerName", scorer_name_, "BEND_0"s);*/
  return EVENT_READER_OK;
}

DECLARE_GEANT4_EVENT_READER(BDSIMEventGenerator)  // add to the factory
