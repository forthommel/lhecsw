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

#ifndef Generator_Common_IHepMCProviderTool_h
#define Generator_Common_IHepMCProviderTool_h

#include <GaudiKernel/IAlgTool.h>

namespace HepMC3 {
  class GenEvent;
}

/// HepMC3 data providers (MC generators, ...) interfacing base class
class IHepMCProviderTool : virtual public IAlgTool {
public:
  DeclareInterfaceID(IHepMCProviderTool, 3, 0);
  virtual StatusCode getNextEvent(HepMC3::GenEvent&) = 0;
};

#endif
