////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2022 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include "input_data.hpp"
#include "switcher.hpp"
#include "utils.hpp"

////////////////////////////////////////////////////////////////////////////////
namespace atem
{

////////////////////////////////////////////////////////////////////////////////
switcher::switcher(asio::io_context& ctx, string hostname, atem::port port) :
    sess_{ ctx, std::move(hostname), port },
    mes_ { sess_ },
    ins_ { sess_ },
    auxs_{ sess_ }
{
    sess_.on_connected([=]{ maybe_call(conn_cb_); });

    sess_.on_disconnected([=]()
    {
        initialized_ = false;
        maybe_call(awol_cb_);
    });

    sess_.on_connect_failed([=]{ maybe_call(failed_cb_); });

    sess_.on_recv_version([=](int major, int minor)
    {
        ver_.major = major;
        ver_.minor = minor;
    });

    sess_.on_recv_prod_info([=](string_view s){ prod_info_ = s; });

    sess_.on_recv_top([=](int mes, int auxs, const vec<input_data>& ins)
    {
        mes_ .reset(mes);
        ins_ .reset(ins);
        auxs_.reset(auxs);
    });

    sess_.on_recv_init_done([=]()
    {
        initialized_ = true;
        maybe_call(init_cb_);
    });
}

////////////////////////////////////////////////////////////////////////////////
void switcher::connect()
{
    ver_ = version{ };
    prod_info_.clear();

    mes_ .reset();
    ins_ .reset();
    auxs_.reset();

    sess_.connect();
}

////////////////////////////////////////////////////////////////////////////////
}
