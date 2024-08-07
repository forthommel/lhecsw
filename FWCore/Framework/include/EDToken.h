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

#ifndef FWCore_Framework_EDToken_h
#define FWCore_Framework_EDToken_h

#include <k4FWCore/DataHandle.h>

namespace edm {
  template <typename T>
  class EDTokenT {
  public:
    EDTokenT() = default;
    EDTokenT(const EDTokenT<T>&) noexcept = default;
    EDTokenT(EDTokenT<T>&&) noexcept = default;

    template <typename U>
    explicit EDTokenT(U&& adapter) : EDTokenT(adapter.template consumes<T>()) {}

    template <typename U>
    constexpr EDTokenT& operator=(U&& adapter) {
      EDTokenT<T> temp(adapter.template consumes<T>());
      data_ = temp.data_;
      return *this;
    }

    EDTokenT(EDTokenT<T>& oth) noexcept : data_{oth.data_} {}
    EDTokenT(const EDTokenT<T>&& oth) noexcept : data_{oth.data_} {}

    EDTokenT& operator=(EDTokenT<T>& oth) { return (*this = const_cast<const EDTokenT<T>&>(oth)); }

  protected:
    virtual void initialise() = 0;

    mutable DataHandle<T> data_;  ///< data consumed
    const std::string label_;
  };
}  // namespace edm

#endif
