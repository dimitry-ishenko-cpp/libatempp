////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2022 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include "packet.hpp"
#include "session.hpp"
#include "utils.hpp"

#include <chrono>
#include <iostream>

////////////////////////////////////////////////////////////////////////////////
namespace atem
{

using asio::ip::udp;
using namespace std::chrono_literals;

// "magic" session id to initiate connection with ATEM
constexpr uint16 init_sess_id = 0x1337;

////////////////////////////////////////////////////////////////////////////////
session::session(asio::io_context& ctx, std::string hostname, port p) :
    hostname_{ std::move(hostname) }, port_{ p },
    socket_{ ctx }, timer_{ ctx }
{ }

////////////////////////////////////////////////////////////////////////////////
void session::connect()
{
    if(!connected_)
    {
        id_ = init_sess_id;
        packet_id_ = 0;

        udp::resolver resolver{ socket_.get_executor() };
        auto ep = *resolver.resolve(udp::v4(), hostname_, std::to_string(port_)).begin();

        socket_.connect(ep);
        async_wait();

        // initiate connection with ATEM
        socket_.send( packet::init(id_).to_buffer() );

        timer_.expires_after(1000ms);
        timer_.async_wait([=](asio::error_code ec){ if(!ec) conn_failed(); });
    }
}

////////////////////////////////////////////////////////////////////////////////
void session::disconnect()
{
    if(connected_)
    {
        connected_ = false;

        timer_.cancel();
        socket_.close();

        maybe_call(disconnected_cb_);
    }
}

////////////////////////////////////////////////////////////////////////////////
void session::on_connected(cb<void()> cb)
{
    connected_cb_ = std::move(cb);
}

////////////////////////////////////////////////////////////////////////////////
void session::on_disconnected(cb<void()> cb)
{
    disconnected_cb_ = std::move(cb);
}

////////////////////////////////////////////////////////////////////////////////
void session::on_connect_failed(cb<void()> cb)
{
    conn_failed_cb_ = std::move(cb);
}

////////////////////////////////////////////////////////////////////////////////
/*void session::send_packet(cmd c, payload p)
{
    packet pkt{ ping_packet, id_, ++pkt_id_ };
    pkt.add_payload(c, p);

    socket_.send(pkt.to_buffer());
}*/

////////////////////////////////////////////////////////////////////////////////
void session::conn_failed()
{
    timer_.cancel();
    socket_.close();

    maybe_call(conn_failed_cb_);
}

////////////////////////////////////////////////////////////////////////////////
void session::async_wait()
{
    socket_.async_wait(udp::socket::wait_read, [=](asio::error_code ec)
    {
        if(!ec)
        {
            raw_data data(socket_.available(), '\0');
            socket_.receive(asio::buffer(data));

            auto pkt = packet::recv(std::move(data));
            if(!pkt.invalid())
            {
                // match ATEM session id
                id_ = pkt.sess_id();

                if(pkt.is(atem::init))
                {
                    // ack init packet with packet id 0
                    socket_.send( packet::pong(id_, 0).to_buffer() );
                }
                else
                {
                    if(pkt.is(atem::ping))
                    {
                        timer_.expires_after(1000ms);
                        timer_.async_wait([=](asio::error_code ec){ if(!ec) disconnect(); });

                        if(!connected_)
                        {
                            connected_ = true;
                            maybe_call(connected_cb_);
                        }

                        if(pkt.empty())
                        {
                            // ack only empty packets
                            socket_.send( packet::pong(id_, pkt.id()).to_buffer() );
                        }
                    }

                    // TODO: We assume packets don't get lost and ignore
                    // the ones marked as "resend".
                    // Ideally we should keep track of packet ids that were
                    // received and ignore the ones with dup id.
                    //
                    if(!pkt.is(atem::resend)) for(;;)
                    {
                        auto [ cmd1, payload1 ] = pkt.next_payload();
                        if(cmd1.invalid()) break;
#if 0
                        // for debugging only
                        auto [ c0, c1, c2, c3 ] = c.to_chars();
                        std::cerr << c0 << c1 << c2 << c3 << ": " << payload.size() << std::endl;
#endif
                        // process cmd1, payload1
                    }
                }
            }

            async_wait();
        }
    });
}

////////////////////////////////////////////////////////////////////////////////
}
