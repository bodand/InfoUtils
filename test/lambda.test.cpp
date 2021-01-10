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

// stdlib
#include <numeric>

// test'd
#include <info/lambda.hpp>
using namespace info;

TEST_CASE("info::lambda tests", "[lambda]") {
    SECTION("lambda allows recursion") {
        int call_count = 0;
        auto sut = lambda([&call_count](auto self, int call_times) -> void {
          ++call_count;

          if (call_times != 0)
              self(call_times - 1);
        });

        sut(4);

        CHECK(call_count == 5);
    }

    SECTION("lambda doesn't disturb return types and parameters") {
        auto sut = lambda([](auto self, auto n) {
          if (n == 0)
              return std::vector<decltype(n)>{n};

          auto ret = self(n - 1);
          ret.push_back(n);
          return ret;
        });
        unsigned n = 6;

        auto got = sut(n);

        std::vector<unsigned> exp(n + 1);
        std::iota(exp.begin(), exp.end(), 0);

        CHECK_THAT(got, Catch::Equals(exp));
    }
}
