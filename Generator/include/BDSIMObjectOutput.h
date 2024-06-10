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

#ifndef Generator_include_BDSIMObjectOutput_h
#define Generator_include_BDSIMObjectOutput_h

#include <BDSOutput.hh>

class BDSIMObjectOutput : public BDSOutput {
public:
  explicit BDSIMObjectOutput();

  void NewFile() override {}
  void CloseFile() override {}

private:
  void WriteHeader() override {}
  void WriteHeaderEndOfFile() override {}
  void WriteParticleData() override {}
  void WriteBeam() override {}
  void WriteOptions() override {}
  void WriteModel() override {}
  void WriteFileEventLevel() override {}
  void WriteFileRunLevel() override {}
};

#endif
