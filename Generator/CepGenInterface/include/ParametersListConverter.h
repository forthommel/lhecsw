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

#ifndef Generator_CepGenInterface_ParametersListConverter_h
#define Generator_CepGenInterface_ParametersListConverter_h

#include <CepGen/Core/ParametersList.h>

#include <nlohmann/json.hpp>

namespace cepgen {
  class ParametersListConverter {
  public:
    explicit ParametersListConverter(const std::string& json_file);

    operator ParametersList() const { return plist_; }

  private:
    ParametersList parse(const nlohmann::json&);

    ParametersList plist_;
  };
}  // namespace cepgen

#endif
