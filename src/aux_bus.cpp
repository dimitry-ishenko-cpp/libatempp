////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2022 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include "aux_bus.hpp"
#include "session.hpp"

////////////////////////////////////////////////////////////////////////////////
namespace atem
{

////////////////////////////////////////////////////////////////////////////////
void aux_bus::set_src(input_id id)
{
    sess_.get().set_src(num_, id);
}

////////////////////////////////////////////////////////////////////////////////
void aux_bus::change_src(input_id id)
{
    src_ = id;
    maybe_call(src_chng_cb_, id);
}

////////////////////////////////////////////////////////////////////////////////
}
