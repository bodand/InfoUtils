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
// Created by bodand on 2020-05-01.
//

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wused-but-marked-unused"
#pragma ide diagnostic ignored "MemberFunctionCanBeStaticInspection"
#pragma ide diagnostic ignored "cert-err58-cpp"
#pragma once

#include "assertion.hpp"
#include <info/functor.hpp>

BOOST_AUTO_TEST_SUITE(Info)
  BOOST_AUTO_TEST_SUITE(Utils)
    using namespace info;

    struct foo_ {
        int operator()(int, char) {
            return ++i;
        }

        int i{0};
    };

    int plain_fun(int) {
        return 1;
    }

    BOOST_AUTO_TEST_CASE(s_functor_is_callable) {
        functor<int(int, char)> fun{foo_{}};

        BOOST_CHECK_EQUAL(fun(1, 'a'), 1);
    }

    BOOST_AUTO_TEST_CASE(s_functor_is_callable_with_plain_function) {
        functor<int(int)> fun{plain_fun};

        BOOST_CHECK_EQUAL(fun(1), 1);
    }

    BOOST_AUTO_TEST_CASE(s_functor_keeps_functors_state) {
        functor<int(int, char)> fun{foo_{}};

        BOOST_CHECK_EQUAL(fun(1, '?'), 1);
        BOOST_CHECK_EQUAL(fun(1, '?'), 2);
        BOOST_CHECK_EQUAL(fun(1, '?'), 3);
    }

  BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
