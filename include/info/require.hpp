//// BSD 3-Clause License
// 
// Copyright (c) 2020, bodand
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
// 
// 1. Redistributions of source code must retain the above copyright notice, this
//    list of conditions and the following disclaimer.
// 
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
// 
// 3. Neither the name of the copyright holder nor the names of its
//    contributors may be used to endorse or promote products derived from
//    this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

//
// Created by bodand on 2020-03-21.
//

#pragma once

// stdlib
#include <exception>
#include <iostream>

// boost
#include <boost/assert.hpp>

namespace info {
  constexpr void require(bool, std::string_view = "");
  constexpr void require_not(bool, std::string_view = "");
}

constexpr void info::require_not(bool expr, std::string_view msg) {
    BOOST_ASSERT_MSG(!expr, "The require_not parameter must be false");
    require(!expr, msg);
}

constexpr void info::require(bool expr, std::string_view msg) {
    BOOST_ASSERT_MSG(expr, "The require parameter must be true");
    if (!
#ifdef __GNUC__
           __builtin_expect(expr, 1)
#else
        expr
#endif
           ) {
        std::cerr << "Requirement failed" << (!msg.empty() ? msg : "") << std::endl;
        std::terminate();
    }
}
