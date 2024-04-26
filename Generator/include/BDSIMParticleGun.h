#ifndef Generator_BDSIMParticleGun_h
#define Generator_BDSIMParticleGun_h

#include <DDG4/Geant4InputAction.h>
#include <TFile.h>

#include <BDSOutputROOTEventSampler.hh>

class TTree;

class BDSIMParticleGun : public dd4hep::sim::Geant4EventReader {
public:
  explicit BDSIMParticleGun(const std::string& file_tree_name);
  virtual ~BDSIMParticleGun() = default;

  using Particles = dd4hep::sim::Geant4InputAction::Particles;
  using Vertices = dd4hep::sim::Geant4InputAction::Vertices;

  EventReaderStatus moveToEvent(int event_number) override;
  EventReaderStatus readParticles(int event_number, Vertices& vertices, Particles& particles) override;
  EventReaderStatus skipEvent() override { return EVENT_READER_OK; }

private:
  int event_number_{-1};
  unsigned long long max_events_{0ull};
  std::unique_ptr<TFile> file_;
  TTree* tree_{nullptr};                                // NOT owning
  BDSOutputROOTEventSampler<float>* sampler_{nullptr};  // NOT owning
};

#endif
