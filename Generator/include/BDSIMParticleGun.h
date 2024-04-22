#ifndef Generator_BDSIMParticleGun_h
#define Generator_BDSIMParticleGun_h

#include <DDG4/Geant4InputAction.h>
#include <TFile.h>

class TTree;

class BDSIMParticleGun : public dd4hep::sim::Geant4EventReader {
public:
  explicit BDSIMParticleGun(const std::string& file_tree_name);

  dd4hep::sim::Geant4EventReader::EventReaderStatus moveToEvent(int event_number) override;
  dd4hep::sim::Geant4EventReader::EventReaderStatus readParticles(
      int event_number,
      dd4hep::sim::Geant4InputAction::Vertices& vertices,
      dd4hep::sim::Geant4InputAction::Particles& particles) override;

private:
  std::unique_ptr<TFile> file_;
  TTree* tree_{nullptr};  // NOT owning
};

#endif
