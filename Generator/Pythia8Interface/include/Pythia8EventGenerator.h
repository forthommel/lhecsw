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

#ifndef Generator_Pythia8Interface_Pythia8EventGenerator_h
#define Generator_Pythia8Interface_Pythia8EventGenerator_h

#include <GaudiAlg/GaudiTool.h>

#include "Generator/Common/include/IHepMCProviderTool.h"

namespace HepMC3 {
  class GenEvent;
  class Pythia8ToHepMC3;
}  // namespace HepMC3

namespace Pythia8 {
  class Pythia;
}

class Pythia8EventGenerator : public GaudiTool, virtual public IHepMCProviderTool {
public:
  explicit Pythia8EventGenerator(const std::string&, const std::string&, const IInterface*);
  virtual ~Pythia8EventGenerator() = default;

  StatusCode initialize() override;
  StatusCode finalize() override;
  StatusCode getNextEvent(HepMC3::GenEvent&) override;

private:
  std::unique_ptr<Pythia8::Pythia> pythia_;
  std::unique_ptr<HepMC3::Pythia8ToHepMC3> hepmc_;

  Gaudi::Property<std::string> cmds_file_;                    ///< Pythia8 commands file to be applied
  Gaudi::Property<std::vector<std::string> > cmds_preinit_;   ///< Pythia8 commands applied before initialisation
  Gaudi::Property<std::vector<std::string> > cmds_postinit_;  ///< Pythia8 commands applied after initialisation
};

#endif
