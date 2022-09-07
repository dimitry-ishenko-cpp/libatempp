////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2022 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include "packet.hpp"
#include "utils.hpp"

#include <initializer_list>

////////////////////////////////////////////////////////////////////////////////
namespace atem
{

////////////////////////////////////////////////////////////////////////////////
// packet header size
constexpr size_t head_size = 12;

// packet header
//
// +--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+
// |    0   |    1   |    2   |    3   |    4   |    5   |    6   |    7   |    8   |    9   |   10   |   11   |
// +--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+
// |76543210|76543210|76543210|76543210|76543210|76543210|76543210|76543210|76543210|76543210|76543210|76543210|
// +--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+
//  ||||||          | |  hi       lo  | |  hi       lo  |                                     |  hi       lo  |
//  |||||+-- size --+ +--- sess_id ---+ +--- pong_id ---+                                     +--- ping_id ---+
//  |||||
//  ||||+--- ping
//  |||+---- init
//  ||+----- resend
//  |+------ ?
//  +------- pong

// "magic" init packet payload
constexpr std::initializer_list<char> init_payload{ 1, 0, 0, 0, 0, 0, 0, 0 };

////////////////////////////////////////////////////////////////////////////////
packet packet::init(uint16 sess_id)
{
    packet pkt{ sess_id };
    pkt.set(atem::init);
    // add "magic" init packet payload
    pkt.data_.insert(pkt.data_.end(), init_payload.begin(), init_payload.end());
    return pkt;
}

////////////////////////////////////////////////////////////////////////////////
packet packet::ping(uint16 sess_id, uint16 ping_id)
{
    packet pkt{ sess_id };
    pkt.set(atem::ping);
    pkt.set_ping_id(ping_id);
    return pkt;
}

////////////////////////////////////////////////////////////////////////////////
packet packet::pong(uint16 sess_id, uint16 pong_id)
{
    packet pkt{ sess_id };
    pkt.set(atem::pong);
    pkt.set_pong_id(pong_id);
    return pkt;
}

////////////////////////////////////////////////////////////////////////////////
packet packet::recv(raw_data data)
{
    packet pkt;
    pkt.data_ = std::move(data);
    return pkt;
}

////////////////////////////////////////////////////////////////////////////////
bool packet::invalid() const
{
    return data_.size() < head_size;
}

////////////////////////////////////////////////////////////////////////////////
bool packet::empty() const
{
    return data_.size() <= head_size;
}

////////////////////////////////////////////////////////////////////////////////
bool packet::is(packet_type type) const
{
    return data_[0] & type;
}

////////////////////////////////////////////////////////////////////////////////
uint16 packet::sess_id() const
{
    return to_uint16(data_[2], data_[3]);
}

////////////////////////////////////////////////////////////////////////////////
uint16 packet::id() const
{
    return to_uint16(data_[10], data_[11]);
}

////////////////////////////////////////////////////////////////////////////////
std::tuple<cmd, raw_view> packet::next_payload()
{
    cmd cmd1;
    raw_view payload1;

    // initialize payload_ on first use
    // NB: data_ cannot be modified after this call
    if(!payload_.data())
    {
        payload_ = data_;
        payload_.remove_prefix(head_size);
    }

    if(payload_.size() >= 2)
    {
        size_t size = to_uint16(payload_[0], payload_[1]);
        if(size >= 8 && size <= payload_.size())
        {
            cmd1 = payload_.substr(4, 4); // skip size and 2 more bytes
            payload1 = payload_.substr(8, size - 8);

            payload_.remove_prefix(size);
        }
    }

    return std::make_tuple(cmd1, payload1);
}

////////////////////////////////////////////////////////////////////////////////
void packet::add_payload(cmd cmd1, raw_view payload1)
{
    auto p = data_.size();

    data_.resize(p + 8);
    auto size = 8 + payload1.size();

    std::tie(data_[p+0], data_[p+1]) = to_chars(size);
    std::tie(data_[p+4], data_[p+5], data_[p+6], data_[p+7]) = cmd1.to_chars();

    data_.append(payload1);
}

////////////////////////////////////////////////////////////////////////////////
packet::packet(uint16 sess_id) :
    data_(head_size, '\0')
{
    set_sess_id(sess_id);
}

////////////////////////////////////////////////////////////////////////////////
void packet::set(packet_type type)
{
    data_[0] |= type;
}

////////////////////////////////////////////////////////////////////////////////
void packet::set_size()
{
    data_[0] &= '\xf8';
    data_[0] |= to_char((data_.size() >> 8) & 0x7);
    data_[1] = to_char(data_.size());
}

////////////////////////////////////////////////////////////////////////////////
void packet::set_sess_id(uint16 id)
{
    std::tie(data_[2], data_[3]) = to_chars(id);
}

////////////////////////////////////////////////////////////////////////////////
void packet::set_ping_id(uint16 id)
{
    std::tie(data_[10], data_[11]) = to_chars(id);
}

////////////////////////////////////////////////////////////////////////////////
void packet::set_pong_id(uint16 id)
{
    std::tie(data_[4], data_[5]) = to_chars(id);
}

////////////////////////////////////////////////////////////////////////////////
}
