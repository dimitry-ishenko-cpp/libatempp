////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2022 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#ifndef ATEM_CMD_HPP
#define ATEM_CMD_HPP

////////////////////////////////////////////////////////////////////////////////
#include "types.hpp"
#include "utils.hpp"

#include <algorithm> // std::min
#include <tuple>

////////////////////////////////////////////////////////////////////////////////
namespace atem
{

////////////////////////////////////////////////////////////////////////////////
// 4-byte ATEM command
class cmd
{
public:
    constexpr cmd() : val_{0} { }
    constexpr cmd(string_view p) : val_{0}
    {
        auto end = std::min(p.begin() + 4, p.end());
        for(auto it = p.begin(); it != end; ++it) val_ = (val_ << 8) | *it;
    }

    constexpr bool invalid() const { return val_ == 0; }

    constexpr bool operator< (cmd rhs) const { return val_ < rhs.val_; }
    constexpr bool operator==(cmd rhs) const { return val_== rhs.val_; }

    auto to_chars() const // for debugging
    {
        return std::make_tuple(
            to_char(val_ >> 24), to_char(val_ >> 16), to_char(val_ >>  8), to_char(val_)
        );
    }

private:
    uint32 val_;
};

////////////////////////////////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////
#endif
