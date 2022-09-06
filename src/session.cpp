////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2022 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include "input_data.hpp"
#include "me_data.hpp"
#include "packet.hpp"
#include "session.hpp"
#include "utils.hpp"

#include <chrono>
#include <iostream>

////////////////////////////////////////////////////////////////////////////////
namespace atem
{

////////////////////////////////////////////////////////////////////////////////
using asio::ip::udp;
using namespace std::chrono_literals;

// "magic" session id to initiate connection with ATEM
constexpr uint16 init_sess_id = 0x1337;

////////////////////////////////////////////////////////////////////////////////
constexpr auto InPr_long_name_size = 20;
constexpr auto InPr_name_size = 4;

enum CInL_set_mask : char
{
    CInL_none = 0x00,
    CInL_set_long_name = 0x01,
    CInL_set_name = 0x02,
    CInL_set_port = 0x04,
};

constexpr auto CInL_long_name_size = InPr_long_name_size;
constexpr auto CInL_name_size = InPr_name_size;

////////////////////////////////////////////////////////////////////////////////
inline auto trimmed(std::string_view s)
{
    return s.substr(0, s.find('\0'));
}

////////////////////////////////////////////////////////////////////////////////
session::session(asio::io_context& ctx, std::string hostname, port p) :
    hostname_{ std::move(hostname) }, port_{ p },
    socket_{ ctx }, timer_{ ctx }
{ }

////////////////////////////////////////////////////////////////////////////////
session::~session() { disconnect(); }

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

        maybe_call(awol_cb_);
    }
}

////////////////////////////////////////////////////////////////////////////////
void session::conn_failed()
{
    timer_.cancel();
    socket_.close();

    maybe_call(failed_cb_);
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
                            maybe_call(conn_cb_);
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
                        auto [ c, p ] = pkt.next_payload();
                        if(c.invalid()) break;
#if 0
                        // for debugging only
                        auto [ c0, c1, c2, c3 ] = c.to_chars();
                        std::cerr << c0 << c1 << c2 << c3 << ": " << payload.size() << std::endl;
#endif
                        ////////////////////
                        if(c == cmd{ "_ver" })
                        {
                            if(p.size() >= 4)
                            {
                                int major = to_uint16(p[0], p[1]);
                                int minor = to_uint16(p[2], p[3]);
                                maybe_call(ver_cb_, major, minor);
                            }
                        }

                        ////////////////////
                        else if(c == cmd{ "_pin" })
                        {
                            if(p.size()) maybe_call(info_cb_, trimmed(p));
                        }

                        ////////////////////
                        else if(c == cmd{ "_top" })
                        {
                            if(p.size() >= 12)
                            {
                                mes_data_.clear();
                                int mes = to_uint8(p[0]);
                                for(int i = 0; i < mes; ++i) mes_data_.push_back(me_data{ i });
                            }
                        }

                        ////////////////////
                        else if(c == cmd{ "InPr" })
                        {
                            if(p.size() >= 36)
                            {
                                int id = to_uint16(p[0], p[1]);

                                std::string long_name{ trimmed(p.substr(2, InPr_long_name_size)) };
                                std::string name{ trimmed(p.substr(22, InPr_name_size)) };

                                auto port = static_cast<input_port>(to_uint16(p[30], p[31]));
                                auto type = static_cast<input_type>(to_uint8(p[32]));

                                auto mes = to_uint8(p[35]);

                                ins_data_.push_back(input_data{
                                    id, std::move(name), std::move(long_name), type, port, mes
                                });
                            }
                        }

                        ////////////////////
                        else if(c == cmd{ "InCm" })
                        {
                            maybe_call(done_cb_, mes_data_, ins_data_);
                        }
                    }
                }
            }

            async_wait();
        }
    });
}

////////////////////////////////////////////////////////////////////////////////
}
