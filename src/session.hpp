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

////////////////////////////////////////////////////////////////////////////////
namespace atem
{

////////////////////////////////////////////////////////////////////////////////
struct input_data;

////////////////////////////////////////////////////////////////////////////////
class session
{
    using udp = asio::ip::udp;

public:
    session(asio::io_context&, const udp::endpoint&);

    bool is_online() const { return socket_.is_open(); }
    void on_offline(cb<void()> cb) { off_cb_ = std::move(cb); }

    ////////////////////
    void on_recv_ver(cb<void(size_t major, size_t minor)> cb) { ver_cb_ = std::move(cb); }
    void on_recv_prod_info(cb<void(string_view)> cb) { info_cb_ = std::move(cb); }

    using top_cb = cb<void(size_t mes, size_t auxs, size_t ins)>;
    void on_recv_top(top_cb cb) { top_cb_ = std::move(cb); }

    using in_prop_cb = cb<void(input_data)>;
    void on_recv_in_prop(in_prop_cb cb) { in_prop_cb_ = std::move(cb); }

    void on_recv_init_done(cb<void()> cb) { done_cb_ = std::move(cb); }

    void on_pgm_changed(cb<void( me_num, input_id)> cb) { pgm_chng_cb_ = std::move(cb); }
    void on_pvw_changed(cb<void( me_num, input_id)> cb) { pvw_chng_cb_ = std::move(cb); }
    void on_src_changed(cb<void(aux_num, input_id)> cb) { aux_chng_cb_ = std::move(cb); }

    void set_input_props(input_id, opt<string> name, opt<string> long_name, opt<input_port>);

    void set_pgm(me_num, input_id);
    void set_pvw(me_num, input_id);

    void cut(me_num);
    void auto_trans(me_num);

    void set_src(aux_num, input_id);

private:
    asio::ip::udp::socket socket_;

    cb<void()> off_cb_;
    void close();

    asio::steady_timer timer_;
    void async_wait();

    uint16 id_, packet_id_;

    void send_packet(cmd, raw_view payload);

    ////////////////////
    cb<void(size_t, size_t)> ver_cb_;
    void recv__ver(raw_view);

    cb<void(string_view)> info_cb_;
    void recv__pin(raw_view);

    top_cb top_cb_;
    void recv__top(raw_view);

    in_prop_cb in_prop_cb_;
    void recv_InPr(raw_view);

    cb<void()> done_cb_;
    void recv_InCm(raw_view);

    cb<void(me_num, input_id)> pgm_chng_cb_, pvw_chng_cb_;
    void recv_PrgI(raw_view);
    void recv_PrvI(raw_view);

    cb<void(aux_num, input_id)> aux_chng_cb_;
    void recv_AuxS(raw_view);
};

////////////////////////////////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////
#endif
