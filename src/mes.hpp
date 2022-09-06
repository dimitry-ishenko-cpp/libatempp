////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2022 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#ifndef ATEM_MES_HPP
#define ATEM_MES_HPP

////////////////////////////////////////////////////////////////////////////////
#include "me.hpp"
#include "me_data.hpp"

#include <vector>

////////////////////////////////////////////////////////////////////////////////
namespace atem
{

////////////////////////////////////////////////////////////////////////////////
class session;

////////////////////////////////////////////////////////////////////////////////
class mes
{
public:
    mes() = default;
    mes(session*, int num_mes);

    mes(const mes&) = delete;
    mes& operator=(const mes&) = delete;

    mes(mes&&) = default;
    mes& operator=(mes&&) = default;

    ////////////////////
    auto count() const { return data_.size(); }

    me operator[](int n) { return me{ sess_, &data_[n] }; }
    const me operator[](int n) const { return me{ sess_, &data_[n] }; }

    me get(int n) { return me{ sess_, &data_.at(n) }; }
    const me get(int n) const { return me{ sess_, &data_.at(n) }; }

private:
    session* sess_ = nullptr;
    mutable std::vector<me_data> data_;
};

////////////////////////////////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////
#endif
