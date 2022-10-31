////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2022 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#ifndef ATEM_TYPES_HPP
#define ATEM_TYPES_HPP

////////////////////////////////////////////////////////////////////////////////
#include <cstdint>
#include <functional>
#include <optional>
#include <string>
#include <string_view>
#include <utility> // std::reference_wrapper
#include <vector>

////////////////////////////////////////////////////////////////////////////////
namespace atem
{

////////////////////////////////////////////////////////////////////////////////
template<typename T>
using cb = std::function<T>;

template<typename T>
using opt = std::optional<T>;

template<typename T>
using ref_wrap = std::reference_wrapper<T>;

using size_t = std::size_t;

using string = std::string;
using string_view = std::string_view;

using uint8 = std::uint8_t;
using uint16 = std::uint16_t;
using uint32 = std::uint32_t;

template<typename T>
using vec = std::vector<T>;

////////////////////////////////////////////////////////////////////////////////
enum aux_num{ };
enum in_id  { no_id = -1 };
enum me_num { };

////////////////////////////////////////////////////////////////////////////////
enum input_type
{
    external = 0,
    black = 1,
    color_bars = 2,
    color_gen = 3,
    mp_fill = 4,
    mp_key = 5,
    super_src = 6,
    ext_direct = 7,
    me_out = 128,
    aux = 129,
    mask = 130,
    mv = 131,
};

////////////////////////////////////////////////////////////////////////////////
enum input_port
{
    none = 0,
    sdi = 1,
    hdmi = 2,
    composite = 3,
    component = 4,
    svideo = 5,
    internal = 256,
};

////////////////////////////////////////////////////////////////////////////////
struct version
{
    size_t major = 0;
    size_t minor = 0;
};

using port = uint16;

////////////////////////////////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////
#endif

