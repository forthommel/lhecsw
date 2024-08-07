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

#ifndef FWCore_Framework_EDGetToken_h
#define FWCore_Framework_EDGetToken_h

#include "FWCore/Framework/include/EDToken.h"

namespace edm {
  template <typename T>
  class EDGetTokenT : public EDTokenT<T> {
  public:
    void initialise() override { EDTokenT<T>::data_ = DataHandle<T>{label_, Gaudi::DataHandle::Reader, this}; }
  };
}  // namespace edm

#endif
