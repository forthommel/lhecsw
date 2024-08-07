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

#include <DD4hep/DD4hepUnits.h>
#include <HepMC3/GenEvent.h>
#include <HepMC3/GenParticle.h>
#include <HepMC3/GenVertex.h>

#include "Generator/Common/include/HepMC3EventConverter.h"

using PropertyMask = dd4hep::detail::ReferenceBitMask<int>;

bool HepMC3EventConverter::convert(const HepMC3::GenEvent& hepmc_evt, Vertices& vertices, Particles& particles) const {
  vertices.clear();
  particles.clear();

  const auto normalise_id = [](int id) -> int { return id - 1; };
  const auto momentum_units =
      (hepmc_evt.momentum_unit() == HepMC3::Units::MomentumUnit::GEV ? dd4hep::GeV : dd4hep::MeV) / dd4hep::MeV;
  const auto length_units =
      (hepmc_evt.length_unit() == HepMC3::Units::LengthUnit::MM ? dd4hep::mm : dd4hep::cm) / dd4hep::mm;

  for (const auto& hm_part : hepmc_evt.particles()) {
    const auto id_norm = normalise_id(hm_part->id());
    if (id_norm == 0)  // two-beam system
      continue;
    auto part = dd4hep::sim::Geant4ParticleHandle(new dd4hep::sim::Geant4InputAction::Particle(id_norm));
    if (const auto flow1 = hm_part->attribute_as_string("flow1"), flow2 = hm_part->attribute_as_string("flow2");
        !flow1.empty() || !flow2.empty()) {
      part->colorFlow[0] = std::stoi(flow1);
      part->colorFlow[1] = std::stoi(flow2);
    }
    PropertyMask status(part->status);
    if (hm_part->status() == 1)
      status.set(dd4hep::sim::G4PARTICLE_GEN_STABLE);
    else
      status.set(dd4hep::sim::G4PARTICLE_GEN_OTHER);
    part->genStatus = hm_part->status() & dd4hep::sim::G4PARTICLE_GEN_STATUS_MASK;
    part->pdgID = hm_part->pid();
    if (particle_properties_getter)
      part->charge = particle_properties_getter(hm_part->pid()).charge;
    if (const auto& prod_vtx = hm_part->production_vertex(); prod_vtx) {
      const auto& prod_vtx_pos = prod_vtx->position();
      part->vsx = part->vex = prod_vtx_pos.x() * length_units;
      part->vsy = part->vey = prod_vtx_pos.y() * length_units;
      part->vsz = part->vez = prod_vtx_pos.z() * length_units;
      part->time = part->properTime = prod_vtx_pos.t() * length_units / dd4hep::c_light;
    }
    if (const auto& end_vtx = hm_part->end_vertex(); end_vtx) {
      const auto& end_vtx_pos = end_vtx->position();
      part->vex = end_vtx_pos.x() * momentum_units;
      part->vey = end_vtx_pos.y() * momentum_units;
      part->vez = end_vtx_pos.z() * momentum_units;
    }
    const auto& mom = hm_part->momentum();
    part->psx = part->pex = mom.px() * momentum_units;
    part->psy = part->pey = mom.py() * momentum_units;
    part->psz = part->pez = mom.pz() * momentum_units;
    part->mass = hm_part->generated_mass() * momentum_units;
    dd4hep::printout(dd4hep::INFO,
                     "HepMC3EventConverter::convert",
                     "Added particle #%d with PDG id=%d, charge=%de, colour=%d/%d, status=%d/%d, momentum=(%g, %g, "
                     "%g), mass=%g, time=%g/%g",
                     part->id,
                     part->pdgID,
                     part->charge,
                     part->colorFlow[0],
                     part->colorFlow[1],
                     part->status,
                     part->genStatus,
                     part->psx,
                     part->psy,
                     part->psz,
                     part->mass,
                     part->time,
                     part->properTime);
    particles.emplace_back(part);
  }
  for (const auto& hm_vtx : hepmc_evt.vertices()) {
    if (!hm_vtx)
      dd4hep::except("HepMC3EventConverter::convert", "Invalid vertex retrieved from HepMC3-converted event.");
    if (hm_vtx->id() == -1)
      continue;
    auto* vtx = vertices.emplace_back(new dd4hep::sim::Geant4InputAction::Vertex);
    const auto& pos = hm_vtx->position();
    vtx->x = pos.x() * length_units;
    vtx->y = pos.y() * length_units;
    vtx->z = pos.z() * length_units;
    vtx->time = pos.t() * length_units / dd4hep::c_light;
    for (const auto& pin : hm_vtx->particles_in())
      vtx->in.insert(normalise_id(pin->id()));
    for (const auto& pout : hm_vtx->particles_out())
      vtx->out.insert(normalise_id(pout->id()));
  }
  return true;
}
