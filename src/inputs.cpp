////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2022 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include "input_data.hpp"
#include "inputs.hpp"

////////////////////////////////////////////////////////////////////////////////
namespace atem
{

////////////////////////////////////////////////////////////////////////////////
inputs::inputs(session& sess, const vec<input_data>& data)
{
    for(auto const& data1 : data)
    {
        ins_.emplace_back(sess, data1);
    }
}

size_t inputs::count(input_type type) const
{
    size_t n = 0;
    for(auto const& in : ins_) if(in.type() == type) ++n;
    return n;
}

////////////////////////////////////////////////////////////////////////////////
size_t inputs::count(input_port port) const
{
    size_t n = 0;
    for(auto const& in : ins_) if(in.port() == port) ++n;
    return n;
}

////////////////////////////////////////////////////////////////////////////////
opt<input> inputs::find(in_id id)
{
    for(auto& in : ins_) if(in.id() == id) return in;
    return { };
}

////////////////////////////////////////////////////////////////////////////////
opt<input const> inputs::find(in_id id) const
{
    for(auto const& in : ins_) if(in.id() == id) return in;
    return { };
}

////////////////////////////////////////////////////////////////////////////////
opt<input> inputs::find(input_type type, size_t n)
{
    for(auto& in : ins_) if(in.type() == type && n-- == 0) return in;
    return { };
}

////////////////////////////////////////////////////////////////////////////////
opt<input const> inputs::find(input_type type, size_t n) const
{
    for(auto const& in : ins_) if(in.type() == type && n-- == 0) return in;
    return { };
}

////////////////////////////////////////////////////////////////////////////////
opt<input> inputs::find(input_port port, size_t n)
{
    for(auto& in : ins_) if(in.port() == port && n-- == 0) return in;
    return { };
}

////////////////////////////////////////////////////////////////////////////////
opt<input const> inputs::find(input_port port, size_t n) const
{
    for(auto const& in : ins_) if(in.port() == port && n-- == 0) return in;
    return { };
}

////////////////////////////////////////////////////////////////////////////////
}
