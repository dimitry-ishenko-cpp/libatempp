////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2022 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include "mes.hpp"
#include "session.hpp"

////////////////////////////////////////////////////////////////////////////////
namespace atem
{

////////////////////////////////////////////////////////////////////////////////
mes::mes(session& sess, size_t num_mes)
{
    for(size_t i = 0; i < num_mes; ++i)
        mes_.emplace_back(sess, static_cast<me_num>(i));

    sess.on_pgm_changed([=](me_num me, src_id src)
    {
        if(me < count()) (*this)[me].change_pgm(src);
    });

    sess.on_pvw_changed([=](me_num me, src_id src)
    {
        if(me < count()) (*this)[me].change_pvw(src);
    });
}

////////////////////////////////////////////////////////////////////////////////
}
