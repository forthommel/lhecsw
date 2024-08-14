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

#include <CepGen/Core/Exception.h>

#include "Generator/CepGenInterface/include/ParametersListConverter.h"

using json = nlohmann::json;

namespace cepgen {
  ParametersListConverter::ParametersListConverter(const std::string& json_config)
      : plist_(parse(json::parse(json_config))) {
    CG_DEBUG("ParametersListConverter") << "Parsed parameters list: " << plist_ << ".";
  }

  ParametersList ParametersListConverter::parse(const json& config) {
    ParametersList plist;
    for (const auto& it : config.items()) {
      const auto& key = it.key();
      const auto& val = it.value();
      if (val.is_boolean())
        plist.set(key, val.template get<bool>());
      else if (val.is_number_unsigned())
        plist.set<unsigned long long>(key, val.template get<unsigned>());
      else if (val.is_number_integer())
        plist.set(key, val.template get<int>());
      else if (val.is_number_float())
        plist.set<double>(key, val.template get<float>());
      else if (val.is_string())
        plist.set(key, val.template get<std::string>());
      else if (val.is_array()) {
        if (val.empty()) {
          CG_WARNING("ParametersListConverter:parse")
              << "Cannot determine array type for key='" << key << "'. Array is empty.";
          continue;
        }
        const auto& item = val.at(0);
        if (item.is_number_unsigned())
          plist.set(key, val.template get<std::vector<unsigned> >());
        else if (item.is_number_integer())
          plist.set(key, val.template get<std::vector<int> >());
        else if (item.is_number_float()) {
          const auto& vec = val.template get<std::vector<float> >();
          std::vector<double> vec_dbl(vec.begin(), vec.end());
          plist.set(key, vec_dbl);
        } else if (item.is_string())
          plist.set(key, val.template get<std::vector<std::string> >());
      } else if (val.is_structured())
        plist.set(key, parse(val.template get<json>()));
      else
        CG_ERROR("ParametersListConverter:parse") << "Unknown object with key='" << key << "'.";
    }
    return plist;
  }
}  // namespace cepgen
