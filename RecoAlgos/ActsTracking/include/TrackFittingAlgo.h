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

/*
 * This file is part of the Acts project.
 *
 * Copyright (C) 2020-2024 CERN for the benefit of the Acts project
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef RecoAlgos_ActsTracking_TrackFittingAlgo_h
#define RecoAlgos_ActsTracking_TrackFittingAlgo_h

class TrackFittingAlgo : public IAlgorithm {
public:
  struct Config {};

  explicit TrackFittingAlgo(const Config&, Acts::Logging::Level);

private:
  Config config_;
};

#endif
