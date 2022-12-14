////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2022 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include "mes.hpp"

////////////////////////////////////////////////////////////////////////////////
namespace atem
{

////////////////////////////////////////////////////////////////////////////////
mes::mes(session& sess, size_t mes)
{
    for(size_t i = 0; i < mes; ++i) mes_.emplace_back(sess, static_cast<me_num>(i));
}

////////////////////////////////////////////////////////////////////////////////
void mes::change_pgm(me_num me, input_id id)
{
    get(me).change_pgm(id);
}

////////////////////////////////////////////////////////////////////////////////
void mes::change_pvw(me_num me, input_id id)
{
    get(me).change_pvw(id);
}

////////////////////////////////////////////////////////////////////////////////
}
