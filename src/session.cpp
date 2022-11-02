////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2022 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include "input_data.hpp"
#include "session.hpp"
#include "utils.hpp"

#include <chrono>

////////////////////////////////////////////////////////////////////////////////
namespace atem
{

////////////////////////////////////////////////////////////////////////////////
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
inline auto trimmed(string_view s)
{
    return s.substr(0, s.find('\0'));
}

////////////////////////////////////////////////////////////////////////////////
session::session(asio::io_context& ctx, const udp::endpoint& ep) :
    socket_{ctx},
    timer_{ctx},
    id_{init_sess_id}, packet_id_{0}
{
    socket_.connect(ep);
    async_wait();

    // initiate connection with ATEM
    socket_.send( packet::init(id_).to_buffer() );

    timer_.expires_after(1000ms);
    timer_.async_wait([=](auto ec){ if(!ec) close(); });
}

////////////////////////////////////////////////////////////////////////////////
session::~session() { close(); }

////////////////////////////////////////////////////////////////////////////////
void session::close()
{
    if(socket_.is_open())
    {
        timer_.cancel();
        socket_.close();

        maybe_call(off_cb_);
    }
}

////////////////////////////////////////////////////////////////////////////////
void session::async_wait()
{
    socket_.async_wait(udp::socket::wait_read, [=](auto ec)
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
                        timer_.async_wait([=](auto ec){ if(!ec) close(); });

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
                        auto [ c0, c1, c2, c3 ] = cmd1.to_chars();
                        std::cerr << c0 << c1 << c2 << c3 << ": " << payload1.size() << std::endl;
#endif
                             if(cmd1 == cmd{"_ver"}) recv__ver(payload1);
                        else if(cmd1 == cmd{"_pin"}) recv__pin(payload1);
                        else if(cmd1 == cmd{"_top"}) recv__top(payload1);
                        else if(cmd1 == cmd{"InPr"}) recv_InPr(payload1);
                        else if(cmd1 == cmd{"InCm"}) recv_InCm(payload1);
                        else if(cmd1 == cmd{"PrvI"}) recv_PrvI(payload1);
                        else if(cmd1 == cmd{"PrgI"}) recv_PrgI(payload1);
                        else if(cmd1 == cmd{"AuxS"}) recv_AuxS(payload1);
                    }
                }
            }

            async_wait();
        }
    });
}

////////////////////////////////////////////////////////////////////////////////
void session::send_packet(cmd cmd1, raw_view payload1)
{
    auto pkt = packet::ping(id_, ++packet_id_);
    pkt.add_payload(cmd1, payload1);

    socket_.send(pkt.to_buffer());
}

////////////////////////////////////////////////////////////////////////////////
void session::recv__ver(raw_view p)
{
    if(p.size() >= 4)
    {
        size_t major = to_uint16(p[0], p[1]);
        size_t minor = to_uint16(p[2], p[3]);

        maybe_call(ver_cb_, major, minor);
    }
}

////////////////////////////////////////////////////////////////////////////////
void session::recv__pin(raw_view p)
{
    if(p.size()) maybe_call(info_cb_, trimmed(p));
}

////////////////////////////////////////////////////////////////////////////////
void session::recv__top(raw_view p)
{
    if(p.size() >= 12)
    {
        auto mes = to_uint8(p[0]);
        auto ins = to_uint8(p[1]);
        auto auxs= to_uint8(p[3]);

        maybe_call(top_cb_, mes, auxs, ins);
    }
}

////////////////////////////////////////////////////////////////////////////////
void session::recv_InPr(raw_view p)
{
    if(p.size() >= 36)
    {
        maybe_call(in_prop_cb_, input_data {
            static_cast<input_id>( to_uint16(p[0], p[1]) ),      // id
            string{ trimmed(p.substr(22, InPr_name_size)) },     // name
            string{ trimmed(p.substr(2, InPr_long_name_size)) }, // long_name
            static_cast<input_type>( to_uint8(p[32]) ),          // type
            static_cast<input_port>( to_uint16(p[30], p[31]) ),  // port
            to_uint8(p[35]),                                     // mes
        });
    }
}

////////////////////////////////////////////////////////////////////////////////
void session::recv_InCm(raw_view)
{
    maybe_call(done_cb_);
}

////////////////////////////////////////////////////////////////////////////////
void session::recv_PrgI(raw_view p)
{
    if(p.size() >= 4)
    {
        auto me = static_cast<me_num>(to_uint8(p[0]));
        auto id = static_cast<input_id>(to_uint16(p[2], p[3]));

        maybe_call(pgm_chng_cb_, me, id);
    }
}

////////////////////////////////////////////////////////////////////////////////
void session::recv_PrvI(raw_view p)
{
    if(p.size() >= 4)
    {
        auto me = static_cast<me_num>(to_uint8(p[0]));
        auto id = static_cast<input_id>(to_uint16(p[2], p[3]));

        maybe_call(pvw_chng_cb_, me, id);
    }
}

////////////////////////////////////////////////////////////////////////////////
void session::recv_AuxS(raw_view p)
{
    if(p.size() >= 4)
    {
        auto aux = static_cast<aux_num>(to_uint8(p[0]));
        auto id = static_cast<input_id>(to_uint16(p[2], p[3]));

        maybe_call(aux_chng_cb_, aux, id);
    }
}

////////////////////////////////////////////////////////////////////////////////
void session::set_input_props(input_id id, opt<string> name, opt<string> long_name, opt<input_port> port)
{
    raw_data p(32, '\0');

    char mask = CInL_none;
    if(long_name)
    {
        long_name->resize(CInL_long_name_size);
        mask |= CInL_set_long_name;
        p.replace(4, long_name->size(), *long_name);
    }
    if(name)
    {
        name->resize(CInL_name_size);
        mask |= CInL_set_name;
        p.replace(24, name->size(), *name);
    }
    if(port)
    {
        mask |= CInL_set_port;
        std::tie(p[28], p[29]) = to_chars(*port);
    }

    if(mask != CInL_none)
    {
        p[0] = mask;
        std::tie(p[2], p[3]) = to_chars(id);
        send_packet(cmd{"CInL"}, p);
    }
}

////////////////////////////////////////////////////////////////////////////////
void session::set_pgm(me_num me, input_id id)
{
    raw_data p(4, '\0');
    p[0] = to_char(me);
    std::tie(p[2], p[3]) = to_chars(id);
    send_packet(cmd{"CPgI"}, p);
}

////////////////////////////////////////////////////////////////////////////////
void session::set_pvw(me_num me, input_id id)
{
    raw_data p(4, '\0');
    p[0] = to_char(me);
    std::tie(p[2], p[3]) = to_chars(id);
    send_packet(cmd{"CPvI"}, p);
}

////////////////////////////////////////////////////////////////////////////////
void session::cut(me_num me)
{
    raw_data p(4, '\0');
    p[0] = to_char(me);
    send_packet(cmd{"DCut"}, p);
}

////////////////////////////////////////////////////////////////////////////////
void session::auto_trans(me_num me)
{
    raw_data p(4, '\0');
    p[0] = to_char(me);
    send_packet(cmd{"DAut"}, p);
}

////////////////////////////////////////////////////////////////////////////////
void session::set_src(aux_num aux, input_id id)
{
    raw_data p(4, '\0');
    p[0] = 1;
    p[1] = to_char(aux);
    std::tie(p[2], p[3]) = to_chars(id);
    send_packet(cmd{"CAuS"}, p);
}

////////////////////////////////////////////////////////////////////////////////
}
