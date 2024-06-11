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
#include <TFile.h>
#include <TTree.h>

#include "Generator/include/BDSIMParticleGun.h"

using namespace std::string_literals;
using EventReaderStatus = dd4hep::sim::Geant4EventReader::EventReaderStatus;
using PropertyMask = dd4hep::detail::ReferenceBitMask<int>;

BDSIMParticleGun::BDSIMParticleGun(const std::string& filename)
    : dd4hep::sim::Geant4EventReader(filename), filename_(filename) {
  m_directAccess = true;
  if (file_.reset(new TFile(filename.data())); !file_)
    dd4hep::except("BDSIMParticleGun::BDSIMParticleGun", "Failed to open file '%s'!", filename.data());
}

EventReaderStatus BDSIMParticleGun::moveToEvent(int event_number) {
  if (m_currEvent == event_number)
    return EVENT_READER_OK;
  if (!tree_)
    dd4hep::except("BDSIMParticleGun:::moveToEvent", "No BDSIM event tree defined!");
  if (event_number >= (int)max_events_)
    return EVENT_READER_EOF;
  switch (tree_->GetEntry(event_number)) {
    case -1:  // ROOT I/O error
      return EVENT_READER_IO_ERROR;
    case 0:  // empty event loaded
      return EVENT_READER_ERROR;
    default: {  // successful readout of event
      dd4hep::printout(
          dd4hep::DEBUG, "BDSIMParticleGun::moveToEvent", "Successfully moved to event number %d.", event_number);
      m_currEvent = event_number;
      return EVENT_READER_OK;
    }
  }
}

EventReaderStatus BDSIMParticleGun::readParticles(int event_number, Vertices& vertices, Particles& particles) {
  if (m_currEvent != event_number)
    if (const auto ret = moveToEvent(event_number); ret != EVENT_READER_OK)
      dd4hep::except("BDSIMParticleGun::readParticles", "Failed to retrieve event number %d!", event_number);
  vertices.clear();
  particles.clear();
  for (int i = 0; i < sampler_->n; ++i) {
    dd4hep::sim::Geant4ParticleHandle part(new Particle(i));
    part->id = sampler_->trackID.at(i);
    part->pdgID = sampler_->partID.at(i);
    PropertyMask status(part->status);
    status.set(dd4hep::sim::G4PARTICLE_GEN_STABLE);
    if (const auto parent = sampler_->parentID.at(i); parent == 0)  // beam particle
      status.set(dd4hep::sim::G4PARTICLE_GEN_BEAM);
    else  // secondary emission
      part->parents.insert(parent);
    part->time = part->properTime = 0.;
    auto* vtx = vertices.emplace_back(new Vertex);  // add the vertex
    if ((int)sampler_->charge.size() > i)           // sometimes not filled
      part->charge = sampler_->charge.at(i);
    else {  //FIXME should retrieve this from Geant4/elsewhere...
      const auto pdgid = std::abs(part->pdgID);
      if (pdgid == 11 || pdgid == 13 || pdgid == 15)
        part->charge = -part->pdgID / pdgid;
      else if (pdgid == 2212)
        part->charge = part->pdgID / pdgid;
    }
    const auto ptot = sampler_->p.at(i) / dd4hep::MeV;
    if ((int)sampler_->mass.size() > i)  // sometimes not filled
      part->mass = sampler_->mass.at(i);
    else {  // compute from energy/momentum conservation
      const auto energy = sampler_->energy.at(i) / dd4hep::MeV;
      part->mass = std::sqrt(energy * energy - ptot * ptot);
    }
    part->psx = part->pex = ptot * sampler_->xp.at(i);
    part->psy = part->pey = ptot * sampler_->yp.at(i);
    part->psz = part->pez = ptot * sampler_->zp.at(i);
    part->vsx = part->vex = sampler_->x.at(i) / dd4hep::m;
    part->vsy = part->vey = sampler_->y.at(i) / dd4hep::m;
    part->vsz = part->vez = sampler_->z / dd4hep::m;
    vtx->x = part->vsx;
    vtx->y = part->vsy;
    vtx->z = part->vsz;
    vtx->time = part->time;
    vtx->in.insert(part->id);
    vtx->out.insert(part->id);
    dd4hep::printout(
        dd4hep::INFO,
        "BDSIMParticleGun::readParticles",
        "Added particle #%d with PDG id=%d, charge=%de, status=%d/%d, vertex=(%g, %g, %g), momentum=(%g, %g, %g)",
        part->id,
        part->pdgID,
        part->charge,
        part->status,
        part->genStatus,
        part->vsx,
        part->vsy,
        part->vsz,
        part->psx,
        part->psy,
        part->psz);
    particles.emplace_back(part);
  }
  return EVENT_READER_OK;
}

EventReaderStatus BDSIMParticleGun::setParameters(std::map<std::string, std::string>& parameters) {
  _getParameterValue(parameters, "TreeName", tree_name_, "Event"s);
  _getParameterValue(parameters, "ScorerName", scorer_name_, "BEND_0"s);

  const auto patch_scorer_name = [](const std::string& scorer_name) -> std::string {
    if (scorer_name[scorer_name.size() - 1] == '.')
      return scorer_name;
    return scorer_name + ".";  // patch for BDSIM output
  };

  tree_ = file_->Get<TTree>(tree_name_.data());
  if (const auto ret = tree_->SetBranchAddress(patch_scorer_name(scorer_name_).data(), &sampler_); ret < TTree::kMatch)
    dd4hep::except("BDSIMParticleGun::BDSIMParticleGun",
                   "Failed to load '%s' scorer branch! Return value: %d.",
                   scorer_name_.data(),
                   ret);
  dd4hep::printout(dd4hep::DEBUG,
                   "BDSIMParticleGun::BDSIMParticleGun",
                   "Loaded '%s' scorer in '%s' tree from '%s' file.",
                   scorer_name_.data(),
                   tree_name_.data(),
                   filename_.data());
  max_events_ = tree_->GetEntriesFast();
  return EVENT_READER_OK;
}

DECLARE_GEANT4_EVENT_READER(BDSIMParticleGun)  // add to the factory
