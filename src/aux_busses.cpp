////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2022 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include "aux_busses.hpp"

////////////////////////////////////////////////////////////////////////////////
namespace atem
{

////////////////////////////////////////////////////////////////////////////////
aux_busses::aux_busses(session& sess, size_t auxs)
{
    for(size_t i = 0; i < auxs; ++i) auxs_.emplace_back(sess, static_cast<aux_num>(i));
}

////////////////////////////////////////////////////////////////////////////////
void aux_busses::change_src(aux_num aux, input_id id)
{
    get(aux).change_src(id);
}

////////////////////////////////////////////////////////////////////////////////
}
