////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2022 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include "switcher.hpp"
#include "utils.hpp"

////////////////////////////////////////////////////////////////////////////////
namespace atem
{

////////////////////////////////////////////////////////////////////////////////
switcher::switcher(asio::io_context& ctx, std::string hostname, port p) :
    sess_{ ctx, std::move(hostname), p }
{
    sess_.on_connected([=]{ maybe_call(conn_cb_); });

    sess_.on_disconnected([=]()
    {
        initialized_ = false;

        ver_ = version{ };
        prod_info_.clear();

        maybe_call(awol_cb_);
    });

    sess_.on_connect_failed([=]{ maybe_call(failed_cb_); });

    sess_.on_recv_version([=](int major, int minor)
    {
        ver_.major = major;
        ver_.minor = minor;
    });

    sess_.on_recv_prod_info([=](std::string_view s){ prod_info_ = s; });

    sess_.on_recv_top([=](int num_mes)
    {
        mes_ = atem::mes{ &sess_, num_mes };
    });

    sess_.on_recv_init_done([=]()
    {
        initialized_ = true;
        maybe_call(init_cb_);
    });
}

////////////////////////////////////////////////////////////////////////////////
}
