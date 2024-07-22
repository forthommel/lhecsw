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

#ifndef Generator_BDSIMInterface_BDSIMParticleGun_h
#define Generator_BDSIMInterface_BDSIMParticleGun_h

#include <GaudiAlg/GaudiTool.h>

#include <BDSOutputROOTEventSampler.hh>

#include "Generator/Common/include/IHepMCProviderTool.h"

class TFile;
class TTree;

namespace HepMC3 {
  class GenEvent;
}

class BDSIMParticleGun : public GaudiTool, virtual public IHepMCProviderTool {
public:
  explicit BDSIMParticleGun(const std::string&, const std::string&, const IInterface*);
  virtual ~BDSIMParticleGun() = default;

  StatusCode initialize() override;
  StatusCode finalize() override;
  StatusCode getNextEvent(HepMC3::GenEvent&) override;

private:
  StatusCode readEvent(HepMC3::GenEvent&);

  Gaudi::Property<std::string> filename_, tree_name_, scorer_name_;
  Gaudi::Property<unsigned long long> max_events_{0ull};
  unsigned long long event_number_{0ull};

  std::unique_ptr<TFile> file_;
  TTree* tree_{nullptr};                                // NOT owning
  BDSOutputROOTEventSampler<float>* sampler_{nullptr};  // NOT owning
};

#endif
