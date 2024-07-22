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

#include "Generator/Common/include/IHepMCProviderTool.h"

namespace cepgen {
  class Generator;
}

namespace HepMC3 {
  class GenEvent;
}

class CepGenEventGenerator : public GaudiTool, virtual public IHepMCProviderTool {
public:
  explicit CepGenEventGenerator(const std::string&, const std::string&, const IInterface*);
  virtual ~CepGenEventGenerator() = default;

  StatusCode initialize() override;
  StatusCode finalize() override;
  StatusCode getNextEvent(HepMC3::GenEvent&) override;

private:
  std::unique_ptr<cepgen::Generator> cepgen_;
  std::shared_ptr<HepMC3::GenCrossSection> xsec_;

  Gaudi::Property<int> verbosity_;                           ///< CepGen module verbosity
  Gaudi::Property<std::vector<std::string> > process_cmds_;  ///< CepGen commands to define process
  Gaudi::Property<std::map<std::string, std::vector<std::string> > > modifiers_cmds_;  ///< Modifiers chain
  Gaudi::Property<std::map<std::string, std::vector<std::string> > > outputs_cmds_;    ///< Outputs chain
};
