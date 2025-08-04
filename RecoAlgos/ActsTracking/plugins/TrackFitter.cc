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

#include <Gaudi/Algorithm.h>
#include <Gaudi/Property.h>
#include <edm4hep/TrackerHitCollection.h>
#include <k4FWCore/DataHandle.h>

class TrackFitter : public Gaudi::Algorithm {
public:
  explicit TrackFitter(const std::string& name, ISvcLocator* service_locator)
      : Gaudi::Algorithm(name, service_locator),
        trk_hits_name_{this, "TrackerHits", ""},
        trk_hits_{trk_hits_name_, Gaudi::DataHandle::Reader, this} {}
  virtual ~TrackFitter() = default;

  StatusCode initialize() override {
    if (const auto status = Gaudi::Algorithm::initialize(); !status.isSuccess())
      return status;
    return StatusCode::SUCCESS;
  }
  StatusCode execute(const EventContext&) const override {
    std::unordered_map<int, std::size_t> edm4hepParticleMap;
    for (const auto& hit : *trk_hits_.get()) {
      /*auto acts_hit = convertHit(
          hit,
          [&](const auto& inParticle) {
            if (auto it = edm4hepParticleMap.find(inParticle.getObjectID().index); it == edm4hepParticleMap.end())
              throw std::runtime_error("SimHit has source particle that we did not see before");
            const auto& particle = unordered.at(it->second);
            return particle.particleId();
          },
          [&](std::uint64_t cellId) {
            const auto& vm = m_cfg.dd4hepDetector->geometryService->detector().volumeManager();
            const auto detElement = vm.lookupDetElement(cellId);
            if (auto it = m_surfaceMap.find(detElement.key()); it == m_surfaceMap.end())
              throw std::runtime_error("Unable to find surface for detElement '" + detElement.name() +
                                       "' with cellId " + std::to_string(cellId));
            return it->second->geometryId();
          });*/
    }
    return StatusCode::SUCCESS;
  }
  StatusCode finalize() override { return Gaudi::Algorithm::finalize(); }

private:
  using MapParticleIdFrom = std::function<ActsFatras::Barcode(const edm4hep::MCParticle& particle)>;
  using MapGeometryIdFrom = std::function<Acts::GeometryIdentifier(std::uint64_t cellId)>;
  ActsFatras::Hit convertHit(const edm4hep::TrackerHit& hit,
                             const MapParticleIdFrom& particle_mapper,
                             const MapGeometryIdFrom& geometry_mapper) {
    const ActsFatras::Barcode part_id = particle_mapper(hit.getMCParticle());
    const auto momentum =
        Acts::Vector3{hit.getMomentum().x * 1_GeV, hit.getMomentum().y * 1_GeV, hit.getMomentum().z * 1_GeV};
    const auto energy = std::hypot(momentum.norm(), rom.getMCParticle().getMass() * 1_GeV);
    const auto hit_pos = hit.getPosition();
    const auto pos4 = Acts::Vector4{hit_pos.x * 1_mm, hit_pos.y * 1_mm, hit_pos.z * 1_mm, hit.getTime() * 1_ns},
               mom4 = Acts::Vector4{momentum.x(), momentum.y(), momentum.z(), energy};

    Acts::Vector4 delta4 = Acts::Vector4::Zero();
    delta4[Acts::eEnergy] = -hit.getEDep() * Acts::UnitConstants::GeV;

    const Acts::GeometryIdentifier geom_id = geometry_mapper(hit.getCellID());
    const std::int32_t index = -1;  // can extract from time, but we need a complete picture of the trajectory first
    return ActsFatras::Hit(geom_id, part_id, pos4, mom4, mom4 + delta4, index);
  }

  Gaudi::Property<std::string> trk_hits_name_;
  mutable DataHandle<edm4hep::TrackerHitCollection> trk_hits_;
};

DECLARE_COMPONENT(TrackFitter)
