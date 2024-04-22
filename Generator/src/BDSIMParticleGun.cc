#include <TTree.h>

#include "Generator/include/BDSIMParticleGun.h"

using Particles = dd4hep::sim::Geant4InputAction::Particles;
using Vertices = dd4hep::sim::Geant4InputAction::Vertices;

BDSIMParticleGun::BDSIMParticleGun(const std::string& filename, const std::string& treename)
    : dd4hep::sim::Geant4EventReader("BDSIMParticleGun"), file_(new TFile(filename.data())) {
  if (file_)
    tree_ = file_->Get<TTree>(treename.data());
}

dd4hep::sim::Geant4EventReader::EventReaderStatus BDSIMParticleGun::moveToEvent(int event_number) {
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

dd4hep::sim::Geant4EventReader::EventReaderStatus BDSIMParticleGun::readParticles(int event_number,
                                                                                  Vertices& vertices,
                                                                                  Particles& particles) {
  if (const auto ret = moveToEvent(event_number); ret != EventReaderStatus::EVENT_READER_OK)
    return ret;
  return EventReaderStatus::EVENT_READER_OK;
}
