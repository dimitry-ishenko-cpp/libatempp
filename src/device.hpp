////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2022 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#ifndef ATEM_SWITCHER_HPP
#define ATEM_SWITCHER_HPP

////////////////////////////////////////////////////////////////////////////////
#include "aux_busses.hpp"
#include "inputs.hpp"
#include "mes.hpp"
#include "types.hpp"

#include <asio.hpp>
#include <memory>

////////////////////////////////////////////////////////////////////////////////
namespace atem
{

////////////////////////////////////////////////////////////////////////////////
struct input_data;
class session;

////////////////////////////////////////////////////////////////////////////////
class device
{
public:
    device(asio::io_context&, string_view hostname, port = 9910);
    ~device();

    device(device&&);
    device& operator=(device&&);

    ////////////////////
    bool is_online() const;
    void on_offline(cb<void()> cb) { off_cb_ = std::move(cb); }

    bool is_defined() const { return defined_; }
    void on_defined(cb<void()> cb) { def_cb_ = std::move(cb); }

    auto const& protocol() const { return ver_; }
    auto const& prod_info() const { return prod_info_; }

    ////////////////////
    auto& mes() { return *mes_; }
    auto const& mes() const { return *mes_; }

    auto me_count() const { return mes_->count(); }

    auto& me(int n) { return mes_->get(n); }
    auto const& me(int n) const { return mes_->get(n); }

    ////////////////////
    auto& inputs() { return *inputs_; }
    auto const& inputs() const { return *inputs_; }

    auto input_count() const { return inputs_->count(); }

    auto& input(int n) { return inputs_->get(n); }
    auto const& input(int n) const { return inputs_->get(n); }

    auto find_input(input_id id) { return inputs_->find(id); }
    auto find_input(input_id id) const { return inputs_->find(id); }

    auto find_input(input_type t, size_t n = 0) { return inputs_->find(t, n); }
    auto find_input(input_type t, size_t n = 0) const { return inputs_->find(t, n); }

    auto find_input(input_port p, size_t n = 0) { return inputs_->find(p, n); }
    auto find_input(input_port p, size_t n = 0) const { return inputs_->find(p, n); }

    ////////////////////
    auto& aux_busses() { return *auxs_; }
    auto const& aux_busses() const { return *auxs_; }

    auto aux_count() const { return auxs_->count(); }

    auto& aux_bus(int n) { return auxs_->get(n); }
    auto const& aux_bus(int n) const { return auxs_->get(n); }

private:
    std::unique_ptr<session> sess_;

    cb<void()> off_cb_;

    bool defined_ = false;
    cb<void()> def_cb_;

    version ver_;
    string prod_info_;

    vec<input_data> data_;

    std::unique_ptr<atem::mes> mes_;
    std::unique_ptr<atem::inputs> inputs_;
    std::unique_ptr<atem::aux_busses> auxs_;
};

////////////////////////////////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////
#endif
