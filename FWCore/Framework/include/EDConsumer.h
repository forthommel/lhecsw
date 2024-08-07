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

#ifndef FWCore_Framework_EDConsumer_h
#define FWCore_Framework_EDConsumer_h

#include <string>

#include "FWCore/Framework/include/EDGetToken.h"

namespace edm {
  class EDConsumer {
  public:
  protected:
    template <typename T>
    EDGetTokenT<T> consumes(const std::string& tag);
  };
}  // namespace edm

#endif
