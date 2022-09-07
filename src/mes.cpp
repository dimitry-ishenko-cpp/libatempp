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
mes::mes(session& sess, size_t num_mes) :
    sess_{ sess }
{
    for(size_t i = 0; i < num_mes; ++i)
        mes_.emplace_back(sess_.get(), static_cast<me_num>(i));

    sess_.get().on_pgm_changed([=](me_num me, src_id src)
    {
        if(me < count()) maybe_call((*this)[me].pgm_chng_cb_, src);
    });

    sess_.get().on_pvw_changed([=](me_num me, src_id src)
    {
        if(me < count()) maybe_call((*this)[me].pvw_chng_cb_, src);
    });
}

////////////////////////////////////////////////////////////////////////////////
mes::~mes()
{
    sess_.get().on_pgm_changed(nullptr);
    sess_.get().on_pvw_changed(nullptr);
}

////////////////////////////////////////////////////////////////////////////////
}
