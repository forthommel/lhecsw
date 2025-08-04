/*
 * Copyright (c) 2014-2024 Key4hep-Project.
 *
 * This file is part of Key4hep.
 * See https://key4hep.github.io/key4hep-doc/ for further info.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <DD4hep/DD4hepUnits.h>
#include <DD4hep/Detector.h>
#include <DD4hep/Printout.h>
#include <GaudiKernel/MsgStream.h>
#include <GaudiKernel/Service.h>
#include <k4Interface/IGeoSvc.h>

#include <Acts/Definitions/Common.hpp>
#include <Acts/Definitions/Units.hpp>
#include <Acts/Geometry/GeometryContext.hpp>
#include <Acts/Geometry/TrackingGeometry.hpp>
#include <Acts/Material/IMaterialDecorator.hpp>
#include <Acts/Plugins/DD4hep/ConvertDD4hepDetector.hpp>
#include <Acts/Surfaces/Surface.hpp>
#include <Acts/Utilities/Logger.hpp>
#include <Acts/Visualization/GeometryView3D.hpp>
#include <Acts/Visualization/ObjVisualization3D.hpp>

#include "RecoAlgos/ActsTracking/include/IActsGeoSvc.h"

class ActsGeoSvc : public extends<Service, IActsGeoSvc> {
public:
  explicit ActsGeoSvc(const std::string& name, ISvcLocator* service_locator)
      : base_class(name, service_locator),
        log_(msgSvc(), name),
        geom_service_name_{this, "GeoSvcName", "GeoSvc", "The name of the GeoSvc instance"},
        debug_geom_{this, "debugGeometry", false, "Option for geometry debugging"},
        output_filename_{this, "outputFileName", "", "Output file name"},
        geom_service_(geom_service_name_, name) {}

  virtual ~ActsGeoSvc() = default;

  inline StatusCode initialize() override {
    if (const auto status = Service::initialize(); !status.isSuccess())
      return status;
    dd4hep_geom_ = geom_service_->getDetector();
    // necessary?
    // dd4hep_geom_->addExtension<IActsGeoSvc>(this);

    Acts::BinningType bTypePhi = Acts::equidistant, bTypeR = Acts::equidistant, bTypeZ = Acts::equidistant;
    const auto layerEnvelopeR = Acts::UnitConstants::mm, layerEnvelopeZ = Acts::UnitConstants::mm,
               defaultLayerThickness = Acts::UnitConstants::fm;
    auto logger = Acts::getDefaultLogger("k4ActsTracking", acts_logging_level_);
    tracking_geom_ = Acts::convertDD4hepDetector(dd4hep_geom_->world(),
                                                 *logger,
                                                 bTypePhi,
                                                 bTypeR,
                                                 bTypeZ,
                                                 layerEnvelopeR,
                                                 layerEnvelopeZ,
                                                 defaultLayerThickness,
                                                 Acts::sortDetElementsByID,
                                                 geom_context_,
                                                 material_decorator_);

    if (debug_geom_) {  // set geometry debug option
      log_ << MSG::INFO << "Geometry debugging is ON." << endmsg;
      if (createGeoObj().isFailure()) {
        log_ << MSG::ERROR << "Could not create geometry OBJ" << endmsg;
        return StatusCode::FAILURE;
      } else
        log_ << MSG::INFO << "Geometry OBJ SUCCESSFULLY created" << endmsg;
    } else {
      log_ << MSG::VERBOSE << "Geometry debugging is OFF." << endmsg;
      return StatusCode::SUCCESS;
    }
    std::cout << "works!" << std::endl;
    return StatusCode::SUCCESS;
  }
  inline StatusCode finalize() override { return StatusCode::SUCCESS; }

  virtual StatusCode execute() { return StatusCode::SUCCESS; }

  const Acts::TrackingGeometry& trackingGeometry() const override { return *tracking_geom_; }

private:
  /// Convert DD4Hep geometry to acts
  inline StatusCode createGeoObj() {
    if (!tracking_geom_)
      return StatusCode::FAILURE;
    Acts::ObjVisualization3D obj_vis;
    tracking_geom_->visitSurfaces([&](const Acts::Surface* surface) {
      if (surface == nullptr) {
        info() << "Failed to retrieve a surface from object." << endmsg;
        return;
      }
      Acts::GeometryView3D::drawSurface(obj_vis, *surface, geom_context_);
    });
    obj_vis.write(output_filename_);
    log_ << MSG::INFO << output_filename_ << " SUCCESSFULLY written." << endmsg;

    return StatusCode::SUCCESS;
  }

  MsgStream log_;  ///< Gaudi logging output
  dd4hep::Detector* dd4hep_geom_{nullptr};

  Gaudi::Property<std::string> geom_service_name_;
  Gaudi::Property<bool> debug_geom_;              ///< Option for the Debug Geometry
  Gaudi::Property<std::string> output_filename_;  ///< Output file name
  ServiceHandle<IGeoSvc> geom_service_;

  Acts::Logging::Level acts_logging_level_{Acts::Logging::INFO};                 ///< ACTS Logging Level
  Acts::GeometryContext geom_context_;                                           ///< ACTS Tracking Geometry Context
  std::unique_ptr<const Acts::TrackingGeometry> tracking_geom_{nullptr};         ///< ACTS Tracking Geometry
  std::shared_ptr<const Acts::IMaterialDecorator> material_decorator_{nullptr};  ///< ACTS Material Decorator
};

DECLARE_COMPONENT(ActsGeoSvc)
