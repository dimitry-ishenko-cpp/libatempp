////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2022 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include "input_data.hpp"
#include "me_data.hpp"
#include "switcher.hpp"
#include "utils.hpp"

////////////////////////////////////////////////////////////////////////////////
namespace atem
{

////////////////////////////////////////////////////////////////////////////////
switcher::switcher(asio::io_context& ctx, std::string hostname, atem::port port) :
    sess_{ ctx, std::move(hostname), port },
    mes_{ sess_, { } },
    ins_{ sess_, { } }
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

    sess_.on_recv_init_done([=](const vec<me_data>& mes_data, const vec<input_data>& ins_data)
    {
        mes_ = atem::mes{ sess_, mes_data };
        ins_ = atem::inputs{ sess_, ins_data };

        initialized_ = true;
        maybe_call(init_cb_);
    });
}

////////////////////////////////////////////////////////////////////////////////
}
