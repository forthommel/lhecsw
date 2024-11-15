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

#include <DD4hep/DetFactoryHelper.h>

static dd4hep::Ref_t create_detector(dd4hep::Detector& description, xml_h e, dd4hep::Ref_t sens) {
  xml_det_t x_det = e;
  const auto name = x_det.nameStr();
  dd4hep::DetElement sdet(name, x_det.id());
  dd4hep::Material mat(description.material(x_det.materialStr()));

  xml_coll_t c(e, _U(shape));
  if (!c)
    throw std::runtime_error("CircEllipticBeamPipe[" + name + "]> Did not find a proper element!");
  xml_comp_t dim(c);
  const auto z_scaling_for_extrusion = dim.coefficient(1.);

  // circular part definition
  dd4hep::Tube circular_part(dim.rmin1(), dim.rmax1(), dim.z(), 0.5 * M_PI, -0.5 * M_PI);
  // elliptical part definition
  dd4hep::EllipticalTube elliptical_inner_part(dim.rmin2(), dim.rmin1(), dim.z() * z_scaling_for_extrusion),
      elliptical_outer_part(dim.rmax2(), dim.rmax1(), dim.z());
  dd4hep::SubtractionSolid elliptical_full_part(elliptical_outer_part, elliptical_inner_part);
  dd4hep::Box elliptical_subtraction_part(dim.rmax2(), dim.rmax1(), dim.z() * z_scaling_for_extrusion);
  dd4hep::SubtractionSolid elliptical_part(
      elliptical_full_part, elliptical_subtraction_part, dd4hep::Position(-dim.rmax2(), 0., 0.));
  // merging of the two volumes
  dd4hep::UnionSolid rectellipse(
      circular_part, elliptical_part, dd4hep::Position(-dim.rmax2() / 2. + dim.rmax1(), 0., 0.));

  dd4hep::Volume det_vol(name, rectellipse, mat);
  const double z_offset = x_det.hasAttr(_U(z_offset)) ? x_det.z_offset() : 0.;
  dd4hep::PlacedVolume pv = description.pickMotherVolume(sdet).placeVolume(det_vol, dd4hep::Position(0, 0, z_offset));

  sdet.setPlacement(pv);
  det_vol.setVisAttributes(description, x_det.visStr());
  det_vol.setLimitSet(description, x_det.limitsStr());
  det_vol.setRegion(description, x_det.regionStr());
  if (x_det.isSensitive()) {
    dd4hep::SensitiveDetector sd = sens;
    xml_dim_t sd_typ = x_det.child(_U(sensitive));
    det_vol.setSensitiveDetector(sens);
    sd.setType(sd_typ.typeStr());
  }

  if (x_det.hasAttr(_U(id))) {
    int det_id = x_det.id();
    pv.addPhysVolID("system", det_id);
  }
  return sdet;
}

DECLARE_DETELEMENT(Lhe_CircElliptic_BeamPipe, create_detector)
