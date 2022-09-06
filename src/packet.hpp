////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2022 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#ifndef ATEM_PACKET_HPP
#define ATEM_PACKET_HPP

////////////////////////////////////////////////////////////////////////////////
#include "cmd.hpp"
#include "types.hpp"

#include <asio.hpp>
#include <string>
#include <string_view>
#include <tuple>

////////////////////////////////////////////////////////////////////////////////
namespace atem
{

////////////////////////////////////////////////////////////////////////////////
using raw_data = std::string;
using raw_view = std::string_view;

enum packet_type
{
    ping  = 0x08,
    init  = 0x10, // aka "hello" packet
    resend= 0x20,
    pong  = 0x80,
};

////////////////////////////////////////////////////////////////////////////////
class packet
{
public:
    ////////////////////
    packet() = default;
    static packet init(uint16 sess_id);
    static packet ping(uint16 sess_id, uint16 ping_id);
    static packet pong(uint16 sess_id, uint16 pong_id);

    static packet recv(raw_data);

    bool invalid() const;
    bool empty() const;

    bool is(packet_type) const;

    uint16 sess_id() const;
    uint16 id() const;

    std::tuple<cmd, raw_view> next_payload();

    void add_payload(cmd, raw_view);

    auto to_buffer()
    {
        // internally we don't use the size field,
        // but it has to be set before exporting the packet
        set_size();
        return asio::buffer(data_);
    }

protected:
    explicit packet(uint16 sess_id);

    raw_data data_;
    raw_view payload_;

    void set(packet_type);
    void set_size();

    void set_sess_id(uint16);
    void set_ping_id(uint16);
    void set_pong_id(uint16);
};

////////////////////////////////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////
#endif
