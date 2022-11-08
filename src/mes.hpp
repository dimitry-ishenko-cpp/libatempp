////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2022 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#ifndef ATEM_MES_HPP
#define ATEM_MES_HPP

////////////////////////////////////////////////////////////////////////////////
#include "me.hpp"
#include "types.hpp"

////////////////////////////////////////////////////////////////////////////////
namespace atem
{

////////////////////////////////////////////////////////////////////////////////
class session;

////////////////////////////////////////////////////////////////////////////////
class mes
{
public:
    explicit mes(session&, size_t mes);

    ////////////////////
    auto count() const { return mes_.size(); }

    auto& operator[](int n) { return mes_[n]; }
    auto const& operator[](int n) const { return mes_[n]; }

    auto& get(int n) { return mes_.at(n); }
    auto const& get(int n) const { return mes_.at(n); }

    ////////////////////
    auto begin() { return mes_.begin(); }
    auto end() { return mes_.end(); }

    auto begin() const { return mes_.begin(); }
    auto end() const { return mes_.end(); }

    auto cbegin() const { return mes_.begin(); }
    auto cend() const { return mes_.end(); }

    auto rbegin() { return mes_.rbegin(); }
    auto rend() { return mes_.rend(); }

    auto rbegin() const { return mes_.rbegin(); }
    auto rend() const { return mes_.rend(); }

    auto crbegin() const { return mes_.rbegin(); }
    auto crend() const { return mes_.rend(); }

private:
    vec<me> mes_;

    friend class device;
    void change_pgm(me_num, input_id);
    void change_pvw(me_num, input_id);
};

////////////////////////////////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////
#endif
