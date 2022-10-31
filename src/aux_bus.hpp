////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2022 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#ifndef ATEM_AUX_BUS_HPP
#define ATEM_AUX_BUS_HPP

////////////////////////////////////////////////////////////////////////////////
#include "types.hpp"

////////////////////////////////////////////////////////////////////////////////
namespace atem
{

////////////////////////////////////////////////////////////////////////////////
class session;

////////////////////////////////////////////////////////////////////////////////
class aux_bus
{
public:
    aux_bus(session& sess, aux_num num) : sess_{sess}, num_{num} { }

    ////////////////////
    auto num() const { return num_; }

    auto src() const { return src_; }
    void set_src(input_id);

    void on_src_changed(cb<void(input_id)> cb) { src_chng_cb_ = std::move(cb); }

private:
    ref_wrap<session> sess_;
    aux_num num_;
    input_id src_ = no_id;

    cb<void(input_id)> src_chng_cb_;

    friend class aux_busses;
    void change_src(input_id);
};

////////////////////////////////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////
#endif
