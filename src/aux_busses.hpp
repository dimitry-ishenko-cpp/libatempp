////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2022 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#ifndef ATEM_AUX_BUSSES_HPP
#define ATEM_AUX_BUSSES_HPP

////////////////////////////////////////////////////////////////////////////////
#include "aux_bus.hpp"
#include "types.hpp"

////////////////////////////////////////////////////////////////////////////////
namespace atem
{

////////////////////////////////////////////////////////////////////////////////
class session;

////////////////////////////////////////////////////////////////////////////////
class aux_busses
{
public:
    aux_busses(session&, size_t num_auxs);

    ////////////////////
    auto count() const { return auxs_.size(); }

    auto& operator[](int n) { return auxs_[n]; }
    auto const& operator[](int n) const { return auxs_[n]; }

    auto& get(int n) { return auxs_.at(n); }
    auto const& get(int n) const { return auxs_.at(n); }

private:
    vec<aux_bus> auxs_;
};

////////////////////////////////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////
#endif
