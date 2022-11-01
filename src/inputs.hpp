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
    explicit inputs(session& sess, const vec<input_data>&);

    ////////////////////
    auto count() const { return inputs_.size(); }

    auto& operator[](int n) { return inputs_[n]; }
    auto const& operator[](int n) const { return inputs_[n]; }

    auto& get(int n) { return inputs_.at(n); }
    auto const& get(int n) const { return inputs_.at(n); }

    size_t count(input_type) const;
    size_t count(input_port) const;

    opt<input> find(input_id);
    opt<input const> find(input_id) const;

    opt<input> find(input_type, size_t n = 0);
    opt<input const> find(input_type, size_t n = 0) const;

    opt<input> find(input_port, size_t n = 0);
    opt<input const> find(input_port, size_t n = 0) const;

private:
    vec<input> inputs_;
};

////////////////////////////////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////
#endif
