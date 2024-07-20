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

#include <DD4hep/Printout.h>

#include <chrono>

#include "Generator/Pythia6Interface/include/Pythia6Interface.h"

extern "C" {
extern int pychge_(int&);
extern void pyexec_();
extern void pygive_(const char*, int);
extern void pyhepc_(int&);
extern void pyinit_(const char*, const char*, const char*, double*, int, int, int);
extern pythia6::pypars_t pypars_;

/// Override the PYSTOP subrouting to allow throwing exceptions
void pystop_(int& mcod) {
  dd4hep::except("pythia6::PYSTOP", "Error with code %d occurred. Aborting the process execution.", mcod);
}
void pytime_(std::array<int, 6>& idati) {
  const auto now = std::chrono::system_clock::now();
  const auto dp = std::chrono::floor<std::chrono::days>(now);
  const auto ymd = std::chrono::year_month_day(dp);
  const auto time = std::chrono::hh_mm_ss(std::chrono::floor<std::chrono::seconds>(now - dp));
  idati[0] = (int)ymd.year();
  idati[1] = (unsigned)ymd.month();
  idati[2] = (unsigned)ymd.day();
  idati[3] = time.hours().count();
  idati[4] = time.minutes().count();
  idati[5] = time.seconds().count();
}
}

namespace pythia6 {
  int pychge(int pdgid) { return pychge_(pdgid); }

  void pyexec() { pyexec_(); }

  void pygive(const std::string& str) { pygive_(str.data(), str.length()); }

  void pyhepc(int mconv) { pyhepc_(mconv); }

  void pyinit(const std::string& frame, const std::string& beam, const std::string& target, double win) {
    pyinit_(frame.data(), beam.data(), target.data(), &win, frame.length(), beam.length(), target.length());
  }

  pypars_t& pypars() { return pypars_; }
}  // namespace pythia6
