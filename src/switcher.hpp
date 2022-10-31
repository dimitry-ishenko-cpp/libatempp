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
class session;

////////////////////////////////////////////////////////////////////////////////
class switcher
{
public:
    switcher(asio::io_context&, string_view hostname, port = 9910);
    ~switcher();

    ////////////////////
    bool is_connected() const;
    void on_disconnected(cb<void()> cb) { disc_cb_ = std::move(cb); }

    bool is_initialized() const { return initialized_; }
    void on_initialized(cb<void()> cb) { init_cb_ = std::move(cb); }

    auto const& protocol() const { return ver_; }
    auto const& product_info() const { return prod_info_; }

    ////////////////////
    auto& mes() { return mes_; }
    auto const& mes() const { return mes_; }

    auto me_count() const { return mes_.count(); }

    auto& me(int n) { return mes_.get(n); }
    auto const& me(int n) const { return mes_.get(n); }

    ////////////////////
    auto& inputs() { return ins_; }
    auto const& inputs() const { return ins_; }

    auto input_count() const { return ins_.count(); }

    auto& input(int n) { return ins_.get(n); }
    auto const& input(int n) const { return ins_.get(n); }

    auto find_input(input_id id) { return ins_.find(id); }
    auto find_input(input_id id) const { return ins_.find(id); }

    auto find_input(input_type t, size_t n = 0) { return ins_.find(t, n); }
    auto find_input(input_type t, size_t n = 0) const { return ins_.find(t, n); }

    auto find_input(input_port p, size_t n = 0) { return ins_.find(p, n); }
    auto find_input(input_port p, size_t n = 0) const { return ins_.find(p, n); }

    ////////////////////
    auto& aux_busses() { return auxs_; }
    auto const& aux_busses() const { return auxs_; }

    auto aux_count() const { return auxs_.count(); }

    auto& aux_bus(int n) { return auxs_.get(n); }
    auto const& aux_bus(int n) const { return auxs_.get(n); }

private:
    std::unique_ptr<session> sess_;

    cb<void()> disc_cb_;

    bool initialized_;
    cb<void()> init_cb_;

    version ver_;
    string prod_info_;

    atem::mes mes_;
    atem::inputs ins_;
    atem::aux_busses auxs_;
};

////////////////////////////////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////
#endif
