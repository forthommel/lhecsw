#include <DDG4/Factories.h>
#include <DDG4/Geant4InputAction.h>
#include <TTree.h>

#include "Common/include/StringUtils.h"
#include "Generator/include/BDSIMParticleGun.h"

DECLARE_GEANT4_EVENT_READER(BDSIMParticleGun)  // add to the factory

using EventReaderStatus = dd4hep::sim::Geant4EventReader::EventReaderStatus;
using PropertyMask = dd4hep::detail::ReferenceBitMask<int>;

std::string patch_scorer_name(const std::string& scorer_name) {
  auto out = scorer_name;
  if (out[out.size() - 1] != '.')  // patch for BDSIM output
    out += ".";
  return out;
}

BDSIMParticleGun::BDSIMParticleGun(const std::string& file_tree_name) : dd4hep::sim::Geant4EventReader(file_tree_name) {
  std::string filename{file_tree_name}, tree_name{"Event"}, scorer_name{"BEND_0"};
  // unpack the "filename[|tree_name[|scorer_name]]" nomenclature
  if (const auto toks = utils::split(file_tree_name, '|', true); toks.size() > 1) {
    filename = toks.at(0);
    tree_name = toks.at(1);
    if (toks.size() > 2)
      scorer_name = toks.at(2);
  }
  file_.reset(new TFile(filename.data()));
  if (file_) {
    tree_ = file_->Get<TTree>(tree_name.data());
    if (const auto ret = tree_->SetBranchAddress(patch_scorer_name(scorer_name).data(), &sampler_); ret < TTree::kMatch)
      dd4hep::except("BDSIMParticleGun::BDSIMParticleGun",
                     "Failed to load '%s' scorer branch! Return value: %d.",
                     scorer_name.data(),
                     ret);
    dd4hep::printout(dd4hep::INFO,
                     "BDSIMParticleGun::BDSIMParticleGun",
                     "Loaded '%s' scorer in '%s' tree from '%s' file.",
                     scorer_name.data(),
                     tree_name.data(),
                     filename.data());
    max_events_ = tree_->GetEntriesFast();
  } else
    dd4hep::except("BDSIMParticleGun::BDSIMParticleGun", "Failed to open file '%s'!", filename.data());
}

EventReaderStatus BDSIMParticleGun::moveToEvent(int event_number) {
  if (event_number_ == event_number)
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
      event_number_ = event_number;
      return EVENT_READER_OK;
    }
  }
}

EventReaderStatus BDSIMParticleGun::readParticles(int event_number, Vertices& vertices, Particles& particles) {
  if (event_number_ != event_number)
    if (const auto ret = moveToEvent(event_number); ret != EVENT_READER_OK)
      dd4hep::except("BDSIMParticleGun::readParticles", "Failed to retrieve event number %d!", event_number);
  vertices.clear();
  particles.clear();
  for (int i = 0; i < sampler_->n; ++i) {
    auto* part = particles.emplace_back(new Particle(i));
    part->id = sampler_->trackID.at(i);
    part->pdgID = sampler_->partID.at(i);

    PropertyMask status(part->status);
    status.set(dd4hep::sim::G4PARTICLE_GEN_STABLE);
    part->status = 0;
    part->genStatus = 1 & dd4hep::sim::G4PARTICLE_GEN_STATUS_MASK;
    part->time = part->properTime = 0.;
    if ((int)sampler_->charge.size() > i)  // sometimes not filled
      part->charge = sampler_->charge.at(i);
    const auto ptot = sampler_->p.at(i) * dd4hep::GeV;
    if ((int)sampler_->mass.size() > i)  // sometimes not filled
      part->mass = sampler_->mass.at(i);
    else {
      const auto energy = sampler_->energy.at(i) * dd4hep::GeV;
      part->mass = std::sqrt(energy * energy - ptot * ptot);
    }
    part->psx = part->pex = ptot * sampler_->xp.at(i);
    part->psy = part->pey = ptot * sampler_->yp.at(i);
    part->psz = part->pez = ptot * sampler_->zp.at(i);
    part->vsx = part->vex = sampler_->x.at(i) * dd4hep::mm;
    part->vsy = part->vey = sampler_->y.at(i) * dd4hep::mm;
    part->vsz = part->vez = sampler_->z * dd4hep::mm;

    // add the vertex
    auto* vtx = vertices.emplace_back(new Vertex);
    vtx->x = part->vsx;
    vtx->y = part->vsy;
    vtx->z = part->vsz;
    vtx->time = part->time;
    vtx->out.insert(part->id);
  }
  return EVENT_READER_OK;
}
