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

extern "C" {
extern int pychge_(int&);
extern void pyexec_();
extern void pygive_(const char*, int);
extern void pyinit_(const char*, const char*, const char*, double*, int, int, int);
}

namespace pythia6 {
  inline int pychge(int pdgid) { return pychge_(pdgid); }
  inline void pyexec() { pyexec_(); }
  inline void pygive(const std::string& str) { pygive_(str.data(), str.length()); }
  inline void pyinit(const std::string& frame, const std::string& beam, const std::string& target, double win) {
    pyinit_(frame.data(), beam.data(), target.data(), &win, frame.length(), beam.length(), target.length());
  }
}  // namespace pythia6

#endif
