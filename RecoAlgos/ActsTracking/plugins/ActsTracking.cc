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
//#include <k4ActsTracking/IActsGeoSvc.h>

#include <Acts/Geometry/TrackingGeometry.hpp>

class ActsTracking : public Gaudi::Algorithm {
public:
  explicit ActsTracking(const std::string& name, ISvcLocator* service_locator)
      : Gaudi::Algorithm(name, service_locator),
        outputs_cmds_{this, "outputs", {}, "CepGen output->commands to define outputs chain"} {}
  virtual ~ActsTracking() = default;

  StatusCode initialize() override {
    if (const auto status = Gaudi::Algorithm::initialize(); !status.isSuccess())
      return status;
    return StatusCode::SUCCESS;
  }

  inline StatusCode finalize() override { return StatusCode::SUCCESS; }
  inline StatusCode execute(const EventContext&) const override { return StatusCode::SUCCESS; }

private:
  //ToolHandle<IActsGeoSvc> acts_geom_;
  Gaudi::Property<std::map<std::string, std::vector<std::string> > > outputs_cmds_;  ///< Outputs chain
};

DECLARE_COMPONENT(ActsTracking)
