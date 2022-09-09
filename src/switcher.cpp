////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2022 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include "input_data.hpp"
#include "session.hpp"
#include "switcher.hpp"
#include "utils.hpp"

////////////////////////////////////////////////////////////////////////////////
namespace atem
{

////////////////////////////////////////////////////////////////////////////////
switcher::switcher(asio::io_context& ctx, string hostname, atem::port port) :
    sess_{new session{ctx, std::move(hostname), port}},
    mes_ {*sess_},
    ins_ {*sess_},
    auxs_{*sess_}
{
    sess_->on_connected([=]{ maybe_call(conn_cb_); });

    sess_->on_disconnected([=]()
    {
        initialized_ = false;
        maybe_call(awol_cb_);
    });

    sess_->on_connect_failed([=]{ maybe_call(failed_cb_); });

    sess_->on_recv_version([=](int major, int minor)
    {
        ver_.major = major;
        ver_.minor = minor;
    });

    sess_->on_recv_prod_info([=](string_view s){ prod_info_ = s; });

    sess_->on_recv_top([=](size_t mes, size_t auxs, const vec<input_data>& ins)
    {
        mes_ = atem::mes{*sess_, mes};
        ins_ = atem::inputs{*sess_, ins};
        auxs_= atem::aux_busses{*sess_, auxs};
    });

    sess_->on_recv_init_done([=]()
    {
        initialized_ = true;
        maybe_call(init_cb_);
    });

    sess_->on_pgm_changed([=]( me_num  me, in_id id){  mes_.change_pgm( me, id); });
    sess_->on_pvw_changed([=]( me_num  me, in_id id){  mes_.change_pvw( me, id); });
    sess_->on_src_changed([=](aux_num aux, in_id id){ auxs_.change_src(aux, id); });
}

////////////////////////////////////////////////////////////////////////////////
void switcher::connect()
{
    ver_ = version{ };
    prod_info_.clear();

    mes_ = atem::mes{*sess_};
    ins_ = atem::inputs{*sess_};
    auxs_= atem::aux_busses{*sess_};

    sess_->connect();
}

////////////////////////////////////////////////////////////////////////////////
void switcher::disconnect()
{
    sess_->disconnect();
}

////////////////////////////////////////////////////////////////////////////////
bool switcher::is_connected() const
{
    return sess_->is_connected();
}

////////////////////////////////////////////////////////////////////////////////
}
