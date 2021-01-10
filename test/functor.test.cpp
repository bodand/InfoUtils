// BSD 3-Clause License
//
// Copyright (c) 2020-2021, bodand
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

#include <catch2/catch.hpp>

// test'd
#include <info/functor.hpp>

using namespace info;

struct func {
    int operator()(int, char) {
        return ++i;
    }

    int i{0};
};

int plain_fun(int) {
    return 1;
}

TEST_CASE("Functor tests", "[functor]") {
    SECTION("functor is callable") {
        functor<int(int, char)> fun{func{}};

        CHECK(fun(1, '?') == 1);
    }

    SECTION("functor is callable with plain function") {
        functor<int(int)> fun{plain_fun};

        CHECK(fun(1) == 1);
    }

    SECTION("functor keeps the state of the functor") {
        functor<int(int, char)> fun{func{}};

        REQUIRE(fun(1, '?') == 1);
        REQUIRE(fun(1, '?') == 2);
        REQUIRE(fun(1, '?') == 3);
    }
}
