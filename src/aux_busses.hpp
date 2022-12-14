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
    explicit aux_busses(session&, size_t auxs);

    ////////////////////
    auto count() const { return auxs_.size(); }

    auto& operator[](int n) { return auxs_[n]; }
    auto const& operator[](int n) const { return auxs_[n]; }

    auto& get(int n) { return auxs_.at(n); }
    auto const& get(int n) const { return auxs_.at(n); }

    ////////////////////
    auto begin() { return auxs_.begin(); }
    auto end() { return auxs_.end(); }

    auto begin() const { return auxs_.begin(); }
    auto end() const { return auxs_.end(); }

    auto cbegin() const { return auxs_.begin(); }
    auto cend() const { return auxs_.end(); }

    auto rbegin() { return auxs_.rbegin(); }
    auto rend() { return auxs_.rend(); }

    auto rbegin() const { return auxs_.rbegin(); }
    auto rend() const { return auxs_.rend(); }

    auto crbegin() const { return auxs_.rbegin(); }
    auto crend() const { return auxs_.rend(); }

private:
    vec<aux_bus> auxs_;

    friend class device;
    void change_src(aux_num, input_id);
};

////////////////////////////////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////
#endif
