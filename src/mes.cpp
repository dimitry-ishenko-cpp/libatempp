////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2022 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include "mes.hpp"

////////////////////////////////////////////////////////////////////////////////
namespace atem
{

////////////////////////////////////////////////////////////////////////////////
mes::mes(session& sess, int num_mes) :
    sess_{ sess }
{
    for(int n = 0; n < num_mes; ++n) data_.push_back(me_data{ n });
}

////////////////////////////////////////////////////////////////////////////////
}
