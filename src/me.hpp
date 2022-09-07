////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2022 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#ifndef ATEM_ME_HPP
#define ATEM_ME_HPP

////////////////////////////////////////////////////////////////////////////////
#include "types.hpp"

////////////////////////////////////////////////////////////////////////////////
namespace atem
{

////////////////////////////////////////////////////////////////////////////////
class input;
class session;

////////////////////////////////////////////////////////////////////////////////
class me
{
public:
    me(session& sess, me_num num) : sess_{ sess }, num_{ num } { }

    ////////////////////
    auto num() const { return num_; }

    auto pgm() const { return pgm_; }
    void set_pgm(src_id);
    void set_pgm(const input&);

    auto pvw() const { return pvw_; }
    void set_pvw(src_id);
    void set_pvw(const input&);

    void cut();
    void auto_trans();

    void on_pgm_changed(cb<void(src_id)> cb) { pgm_chng_cb_ = std::move(cb); }
    void on_pvw_changed(cb<void(src_id)> cb) { pvw_chng_cb_ = std::move(cb); }

private:
    ref<session> sess_;
    me_num num_;
    src_id pgm_ = no_id;
    src_id pvw_ = no_id;

    cb<void(src_id)> pgm_chng_cb_, pvw_chng_cb_;

    friend class mes;
    void change_pgm(src_id);
    void change_pvw(src_id);
};

////////////////////////////////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////
#endif
