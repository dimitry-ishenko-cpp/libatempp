////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2022 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include "device.hpp"
#include "input_data.hpp"
#include "session.hpp"
#include "utils.hpp"

////////////////////////////////////////////////////////////////////////////////
namespace atem
{

////////////////////////////////////////////////////////////////////////////////
device::device(asio::io_context& ctx, string_view hostname, port p) :
    sess_{std::make_unique<session>(ctx, hostname, p)}
{
    sess_->on_disconnected([=]()
    {
        initialized_ = false;
        maybe_call(disc_cb_);
    });

    sess_->on_recv_version([=](int major, int minor)
    {
        ver_.major = major;
        ver_.minor = minor;
    });

    sess_->on_recv_prod_info([=](string_view s){ prod_info_ = s; });

    sess_->on_recv_top([=](size_t mes, size_t auxs, size_t ins)
    {
        data_ = vec<input_data>(ins);

        mes_   = std::make_unique<atem::mes>(*sess_, mes);
        inputs_= std::make_unique<atem::inputs>(*sess_, data_);
        auxs_  = std::make_unique<atem::aux_busses>(*sess_, auxs);
    });

    sess_->on_recv_in_prop([=](input_data new_d)
    {
        for(auto& d : data_)
            if(d.id == no_id || d.id == new_d.id)
            {
                d = std::move(new_d);
                break;
            }
    });

    sess_->on_recv_init_done([=]()
    {
        initialized_ = true;
        maybe_call(init_cb_);
    });

    sess_->on_pgm_changed([=]( me_num  me, input_id id){  mes_->change_pgm( me, id); });
    sess_->on_pvw_changed([=]( me_num  me, input_id id){  mes_->change_pvw( me, id); });
    sess_->on_src_changed([=](aux_num aux, input_id id){ auxs_->change_src(aux, id); });
}

////////////////////////////////////////////////////////////////////////////////
device::~device() { }

device::device(device&&) = default;
device& device::operator=(device&&) = default;

////////////////////////////////////////////////////////////////////////////////
bool device::is_connected() const
{
    return sess_->is_connected();
}

////////////////////////////////////////////////////////////////////////////////
}
