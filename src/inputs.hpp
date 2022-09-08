////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2022 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#ifndef ATEM_INPUTS_HPP
#define ATEM_INPUTS_HPP

////////////////////////////////////////////////////////////////////////////////
#include "input.hpp"
#include "types.hpp"

////////////////////////////////////////////////////////////////////////////////
namespace atem
{

////////////////////////////////////////////////////////////////////////////////
class session;
struct input_data;

////////////////////////////////////////////////////////////////////////////////
class inputs
{
public:
    explicit inputs(session& sess) : sess_{ sess } { }

    ////////////////////
    auto count() const { return ins_.size(); }

    auto& operator[](int n) { return ins_[n]; }
    auto const& operator[](int n) const { return ins_[n]; }

    auto& get(int n) { return ins_.at(n); }
    auto const& get(int n) const { return ins_.at(n); }

    size_t count(input_type) const;
    size_t count(input_port) const;

    opt<input> find(src_id);
    opt<input const> find(src_id) const;

    opt<input> find(input_type, size_t n = 0);
    opt<input const> find(input_type, size_t n = 0) const;

    opt<input> find(input_port, size_t n = 0);
    opt<input const> find(input_port, size_t n = 0) const;

private:
    ref<session> sess_;
    vec<input> ins_;

    friend class switcher;
    void reset(const vec<input_data>& = { });
};

////////////////////////////////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////
#endif
