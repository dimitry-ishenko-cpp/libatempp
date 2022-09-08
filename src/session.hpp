////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2022 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#ifndef ATEM_SESSION_HPP
#define ATEM_SESSION_HPP

////////////////////////////////////////////////////////////////////////////////
#include "cmd.hpp"
#include "packet.hpp" // raw_view
#include "types.hpp"

#include <asio.hpp>
#include <string>
#include <string_view>

////////////////////////////////////////////////////////////////////////////////
namespace atem
{

////////////////////////////////////////////////////////////////////////////////
using port = uint16;

struct input_data;

////////////////////////////////////////////////////////////////////////////////
class session
{
public:
    session(asio::io_context&, string hostname, port);
    ~session();

    ////////////////////
    void connect();
    void disconnect();

    bool is_connected() const { return connected_; }

    void on_connected(cb<void()> cb) { conn_cb_ = std::move(cb); }
    void on_disconnected(cb<void()> cb) { awol_cb_ = std::move(cb); }
    void on_connect_failed(cb<void()> cb) { failed_cb_ = std::move(cb); }

    ////////////////////
    void on_recv_version(cb<void(int major, int minor)> cb) { ver_cb_ = std::move(cb); }
    void on_recv_prod_info(cb<void(string_view)> cb) { info_cb_ = std::move(cb); }

    using top_cb = cb<void(int mes, size_t auxs, const vec<input_data>&)>;
    void on_recv_top(top_cb cb) { top_cb_ = std::move(cb); }
    void on_recv_init_done(cb<void()> cb) { done_cb_ = std::move(cb); }

    void on_pgm_changed(cb<void( me_num, in_id)> cb) { pgm_chng_cb_ = std::move(cb); }
    void on_pvw_changed(cb<void( me_num, in_id)> cb) { pvw_chng_cb_ = std::move(cb); }
    void on_src_changed(cb<void(aux_num, in_id)> cb) { aux_chng_cb_ = std::move(cb); }

    void set_input_props(in_id, opt<string> name, opt<string> long_name, opt<input_port>);

    void set_pgm(me_num, in_id);
    void set_pvw(me_num, in_id);

    void cut(me_num);
    void auto_trans(me_num);

    void set_src(aux_num, in_id);

private:
    string hostname_;
    port port_;
    asio::ip::udp::socket socket_;

    bool connected_ = false;
    cb<void()> conn_cb_, awol_cb_;

    cb<void()> failed_cb_;
    void conn_failed();

    asio::steady_timer timer_;
    void async_wait();

    uint16 id_, packet_id_;

    void send_packet(cmd, raw_view payload);

    ////////////////////
    cb<void(int, int)> ver_cb_;
    void recv__ver(raw_view);

    cb<void(string_view)> info_cb_;
    void recv__pin(raw_view);

    top_cb top_cb_;
    void recv__top(raw_view);

    vec<input_data> ins_;
    void recv_InPr(raw_view);

    cb<void()> done_cb_;
    void recv_InCm(raw_view);

    cb<void(me_num, in_id)> pgm_chng_cb_, pvw_chng_cb_;
    void recv_PrgI(raw_view);
    void recv_PrvI(raw_view);

    cb<void(aux_num, in_id)> aux_chng_cb_;
    void recv_AuxS(raw_view);
};

////////////////////////////////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////
#endif
