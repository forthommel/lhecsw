#include <DDG4/Factories.h>
#include <TTree.h>

#include "Common/include/StringUtils.h"
#include "Generator/include/BDSIMParticleGun.h"

using EventReaderStatus = dd4hep::sim::Geant4EventReader::EventReaderStatus;
using Particles = dd4hep::sim::Geant4InputAction::Particles;
using Vertices = dd4hep::sim::Geant4InputAction::Vertices;

BDSIMParticleGun::BDSIMParticleGun(const std::string& file_tree_name) : dd4hep::sim::Geant4EventReader(file_tree_name) {
  std::string filename, tree_name;
  if (const auto toks = utils::split(file_tree_name, '|', true); toks.size() > 1) {
    filename = toks.at(0);
    tree_name = toks.at(1);
  } else {
    filename = file_tree_name;
    tree_name = "Event";  // default BDSIM output tree name
  }
  file_.reset(new TFile(filename.data()));
  if (file_)
    tree_ = file_->Get<TTree>(tree_name.data());
}

EventReaderStatus BDSIMParticleGun::moveToEvent(int event_number) {
  if (!tree_)
    throw std::runtime_error("BDSIMParticleGun: No BDSIM event tree defined!");
  if (event_number >= tree_->GetEntriesFast())
    return EventReaderStatus::EVENT_READER_EOF;
  switch (tree_->GetEntry(event_number)) {
    case -1:
      return EventReaderStatus::EVENT_READER_IO_ERROR;
    case 0:
      return EventReaderStatus::EVENT_READER_ERROR;
    default:
      return EventReaderStatus::EVENT_READER_OK;
  }
}

EventReaderStatus BDSIMParticleGun::readParticles(int event_number, Vertices& vertices, Particles& particles) {
  if (const auto ret = moveToEvent(event_number); ret != EventReaderStatus::EVENT_READER_OK)
    return ret;
  return EventReaderStatus::EVENT_READER_OK;
}

DECLARE_GEANT4_EVENT_READER(BDSIMParticleGun)
