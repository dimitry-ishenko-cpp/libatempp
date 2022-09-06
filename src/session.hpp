////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2022 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#ifndef ATEM_SESSION_HPP
#define ATEM_SESSION_HPP

////////////////////////////////////////////////////////////////////////////////
#include "types.hpp"

#include <asio.hpp>
#include <string>
#include <string_view>

////////////////////////////////////////////////////////////////////////////////
namespace atem
{

////////////////////////////////////////////////////////////////////////////////
using port = uint16;

struct me_data;

////////////////////////////////////////////////////////////////////////////////
class session
{
public:
    session(asio::io_context&, std::string hostname, port);
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
    void on_recv_prod_info(cb<void(std::string_view)> cb) { info_cb_ = std::move(cb); }

    using done_cb = cb<void(const vec<me_data>&)>;
    void on_recv_init_done(done_cb cb) { done_cb_ = std::move(cb); }

private:
    std::string hostname_;
    port port_;
    asio::ip::udp::socket socket_;

    bool connected_ = false;
    cb<void()> conn_cb_, awol_cb_;

    cb<void()> failed_cb_;
    void conn_failed();

    asio::steady_timer timer_;
    void async_wait();

    uint16 id_, packet_id_;

    ////////////////////
    cb<void(int, int)> ver_cb_;
    cb<void(std::string_view)> info_cb_;
    done_cb done_cb_;

    vec<me_data> mes_data_;
};

////////////////////////////////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////
#endif
