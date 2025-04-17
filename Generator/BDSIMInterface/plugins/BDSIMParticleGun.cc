/*
 *  LHeC offline simulation and reconstruction software
 *  Copyright (C) 2024-2025  Laurent Forthomme
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

#include <GaudiKernel/AlgTool.h>
#include <HepMC3/FourVector.h>
#include <HepMC3/GenEvent.h>
#include <HepMC3/GenParticle.h>
#include <HepMC3/GenVertex.h>
#include <TFile.h>
#include <TTree.h>

#include <BDSOutputROOTEventSampler.hh>

#include "Generator/Common/include/IHepMCProviderTool.h"

class BDSIMParticleGun : public AlgTool, virtual public IHepMCProviderTool {
public:
  explicit BDSIMParticleGun(const std::string& type, const std::string& name, const IInterface* parent)
      : AlgTool(type, name, parent),
        filename_{this, "filename", "", "BDSIM event filename"},
        tree_name_{this, "treeName", "Event", "BDSIM event tree name"},
        scorer_name_{this, "scorerName", "", "BDSIM scorer name"} {}
  virtual ~BDSIMParticleGun() = default;

  inline StatusCode initialize() override {
    if (const auto status = AlgTool::initialize(); !status.isSuccess())
      return status;
    if (file_.reset(new TFile(filename_.value().data())); !file_) {
      error() << "Failed to open file '" << filename_ << "'!";
      return StatusCode::FAILURE;
    }

    const auto patch_scorer_name = [](const std::string& scorer_name) -> std::string {
      if (scorer_name[scorer_name.size() - 1] == '.')
        return scorer_name;
      return scorer_name + ".";  // patch for BDSIM output
    };

    if (tree_)
      warning() << "Replacing an already existing TTree handle by a new initialisation call.";
    tree_ = file_->Get<TTree>(tree_name_.value().data());
    if (const auto ret = tree_->SetBranchAddress(patch_scorer_name(scorer_name_).data(), &sampler_);
        ret < TTree::kMatch) {
      error() << "Failed to load '" << scorer_name_ << "' scorer branch! Return value: " << ret << ".";
      return StatusCode::FAILURE;
    }

    debug() << "Loaded '" << scorer_name_ << "' scorer in '" << tree_name_ << "' tree from '" << filename_ << "' file.";
    max_events_ = tree_->GetEntriesFast();
    return StatusCode::SUCCESS;
  }
  inline StatusCode finalize() override {
    tree_ = nullptr;
    file_.release();
    return StatusCode::SUCCESS;
  }
  inline StatusCode getNextEvent(HepMC3::GenEvent& hepmc_evt) override {
    if (!tree_) {
      error() << "No BDSIM event tree defined!";
      return StatusCode::FAILURE;
    }
    if (event_number_ >= max_events_) {
      warning() << "End of file reached. Now starting over.";
      event_number_ = 0ull;
    }
    switch (tree_->GetEntry(event_number_)) {
      case -1: {  // ROOT I/O error
        error() << "ROOT I/O error encountered.";
        return StatusCode::FAILURE;
      }
      case 0: {  // empty event loaded
        error() << "An empty event content was loaded.";
        return StatusCode::FAILURE;
      }
      default: {  // successful readout of event
        debug() << "Successfully moved to event number " << event_number_ << ".";
        return readEvent(hepmc_evt);
      }
    }
  }

private:
  StatusCode readEvent(HepMC3::GenEvent&);

  Gaudi::Property<std::string> filename_, tree_name_, scorer_name_;
  Gaudi::Property<unsigned long long> max_events_{0ull};
  unsigned long long event_number_{0ull};

  std::unique_ptr<TFile> file_;
  TTree* tree_{nullptr};                                // NOT owning
  BDSOutputROOTEventSampler<float>* sampler_{nullptr};  // NOT owning
};

DECLARE_COMPONENT(BDSIMParticleGun)

StatusCode BDSIMParticleGun::readEvent(HepMC3::GenEvent& hepmc_evt) {
  hepmc_evt.clear();
  for (int i = 0; i < sampler_->n; ++i) {
    const auto ptot = sampler_->p.at(i) * 1.e3;
    const auto mom = HepMC3::FourVector(ptot * sampler_->xp.at(i),
                                        ptot * sampler_->yp.at(i),
                                        -ptot * sampler_->zp.at(i),
                                        sampler_->energy.at(i) * 1.e3);
    int status{1};
    if (const auto parent = sampler_->parentID.at(i); parent == 0)  // beam particle
      status = -9;
    auto part = std::make_shared<HepMC3::GenParticle>(mom, sampler_->partID.at(i), status);
    const auto vtx_pos = HepMC3::FourVector(sampler_->x.at(i) * 10., sampler_->y.at(i) * 10., -sampler_->z * 10., 0.);
    auto vtx = std::make_shared<HepMC3::GenVertex>(vtx_pos);
    part->production_vertex() = vtx;
    hepmc_evt.add_particle(part);
    info() << "Added particle #" << part->id() << " with PDG id=" << part->pid() << ", status=" << part->status()
           << ", vertex=(" << part->production_vertex()->position().x() << ","
           << part->production_vertex()->position().y() << "," << part->production_vertex()->position().z() << ";"
           << part->production_vertex()->position().t() << "), momentum=(" << part->momentum().px() << ","
           << part->momentum().py() << "," << part->momentum().pz() << ";" << part->momentum().e() << ").";
  }
  return StatusCode::SUCCESS;
}
