////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2022 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#ifndef ATEM_SWITCHER_HPP
#define ATEM_SWITCHER_HPP

////////////////////////////////////////////////////////////////////////////////
#include "session.hpp"
#include "types.hpp"

#include <asio.hpp>
#include <string>

////////////////////////////////////////////////////////////////////////////////
namespace atem
{

////////////////////////////////////////////////////////////////////////////////
struct version
{
    int major = 0;
    int minor = 0;
};

////////////////////////////////////////////////////////////////////////////////
class switcher
{
public:
    switcher(asio::io_context&, std::string hostname, port = 9910);

    switcher(const switcher&) = delete;
    switcher& operator=(const switcher&) = delete;

    switcher(switcher&&) = default;
    switcher& operator=(switcher&&) = default;

    ////////////////////
    void connect() { sess_.connect(); }
    void disconnect() { sess_.disconnect(); }

    bool is_connected() const { return sess_.is_connected(); }

    void on_connected(cb<void()> cb) { conn_cb_ = std::move(cb); }
    void on_disconnected(cb<void()> cb) { awol_cb_ = std::move(cb); }
    void on_connect_failed(cb<void()> cb) { failed_cb_ = std::move(cb); }

    bool is_initialized() const { return initialized_; }
    void on_initialized(cb<void()> cb) { init_cb_ = std::move(cb); }

    auto const& protocol() const { return ver_; }
    auto const& product_info() const { return prod_info_; }

private:
    session sess_;

    cb<void()> conn_cb_, awol_cb_;
    cb<void()> failed_cb_;

    bool initialized_;
    cb<void()> init_cb_;

    version ver_;
    std::string prod_info_;
};

////////////////////////////////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////
#endif
