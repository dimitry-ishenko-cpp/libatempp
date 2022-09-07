////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2022 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include "me_data.hpp"
#include "mes.hpp"
#include "session.hpp"

////////////////////////////////////////////////////////////////////////////////
namespace atem
{

////////////////////////////////////////////////////////////////////////////////
mes::mes(session& sess, const vec<me_data>& data)
{
    for(auto const& data1 : data)
        mes_.emplace_back(sess, data1);

    sess.on_pgm_changed([=](me_num me, src_id src)
    {
        if(me < count()) maybe_call((*this)[me].pgm_chng_cb_, src);
    });

    sess.on_pvw_changed([=](me_num me, src_id src)
    {
        if(me < count()) maybe_call((*this)[me].pvw_chng_cb_, src);
    });
}

////////////////////////////////////////////////////////////////////////////////
}
