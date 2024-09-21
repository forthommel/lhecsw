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

#ifndef Generator_Pythia6Interface_Pythia6Interface_h
#define Generator_Pythia6Interface_Pythia6Interface_h

#include <array>
#include <string>

namespace pythia6 {
  struct pypars_t {
    std::array<int, 200> mstp;
    std::array<double, 200> parp;
    std::array<int, 200> msti;
    std::array<double, 200> pari;
  };
  int pychge(int pdgid);
  void pyexec();
  void pygive(const std::string& str);
  void pyhepc(int mconv);
  void pyinit(const std::string& frame, const std::string& beam, const std::string& target, double win);
  pypars_t& pypars();
}  // namespace pythia6

#endif
