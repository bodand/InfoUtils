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
// Created by bodand on 2020-05-13.
//

#include <catch2/catch.hpp>

// test'd header
#include <info/expected.hpp>

#ifndef USE_UNEXPECTED
#  pragma warning\
    "WARNING: expected.test.cpp Requires the macro USE_UNEXPECTED to be defined to the unexpected variant to use"
#  define USE_UNEXPECTED unexpected
#endif

TEST_CASE("Expected test cases", "[expected][!throws]") {
    // may throw:
    // the tests are written to expect exceptions where they are due
    // but you know, bugs exist
    using namespace info;

    SECTION("Construction behaves correctly") {
        SECTION("From expected value") {
            expected<int, int> exp{55};

            CHECK(*exp == 55);
        }

        SECTION("Copy-assignment from expected value") {
            int a = 218;
            expected<int, int> exp = a;

            REQUIRE(a == 218);
            CHECK(*exp == 218);
        }

        SECTION("Move-assignment from expected value") {
            expected<int, int> exp = 125;

            CHECK(*exp == 125);
        }

        SECTION("From unexpected value") {
            auto un = USE_UNEXPECTED{55};
            expected<int, int> exp{un};

            CHECK(exp.error() == 55);
        }

        SECTION("Copy-assignment from unexpected value") {
            int a = 218;
            expected<int, int> exp = USE_UNEXPECTED{a};

            REQUIRE(a == 218);
            CHECK(exp.error() == 218);
        }

        SECTION("Move-assignment from unexpected value") {
            expected<int, int> exp = USE_UNEXPECTED{125};

            CHECK(exp.error() == 125);
        }

        SECTION("Copy-constructor from expected value containing expected") {
            expected<int, int> exp = 223;
            auto exp_c{exp};

            REQUIRE(*exp == 223);
            CHECK(*exp_c == 223);
        }

        SECTION("Copy-constructor from unexpected value containing expected") {
            expected<int, int> un = USE_UNEXPECTED{444};
            auto un_c{un};

            REQUIRE(un.error() == 444);
            CHECK(un_c.error() == 444);
        }

        SECTION("Move-constructor from expected value containing expected") {
            expected<int, int> exp = 223;
            auto exp_m{std::move(exp)};

            CHECK(*exp_m == 223);
        }

        SECTION("Copy-constructor from unexpected value containing expected") {
            expected<int, int> un = USE_UNEXPECTED{444};
            auto un_m{std::move(un)};

            CHECK(un_m.error() == 444);
        }

        SECTION("Default constructable if expected type is") {
            expected<int, int> exp;
            int def{};

            REQUIRE(def == 0);
            CHECK(*exp == def);
        }
    }

    struct Foo {
        int i;
    };

    expected<int, int> exp;
    expected<Foo, int> fexp{Foo{4}}; // I'm sad
    expected<int, int> un{USE_UNEXPECTED{42}};
    expected<Foo, int> fun{USE_UNEXPECTED{42}};

    SECTION("Boolean conversion") {
        SECTION("Explicit boolean conversion happens when true") {
            CHECK(static_cast<bool>(exp));
        }

        SECTION("Explicit boolean conversion happens when false") {
            CHECK_FALSE(static_cast<bool>(un));
        }
    }

    SECTION("Pseudo-Pointer style access") {
        SECTION("Dereference access returns correct value if successful") {
            CHECK(fexp->i == 4);
        }

        SECTION("Dereference operator throws if unexpected contained") {
            CHECK_THROWS_MATCHES(
                   (void) *un,
                   int,
                   Catch::Predicate<int>([](int i) {
                     return i == 42;
                   })
            );
        }

        SECTION("Dereference access operator throws is unexpected is stored") {
            CHECK_THROWS_MATCHES(
                   (void) fun->i,
                   int,
                   Catch::Predicate<int>([](int i) {
                     return i == 42;
                   })
            );
        }

        SECTION("Apply operator applies function on expected value") {
            auto f = [](int i) { return i * i; };

            auto newexp = exp(f);

            CHECK(static_cast<bool>(newexp) == static_cast<bool>(exp));
            CHECK(*newexp == f(*exp));
        }

        SECTION("Apply operator doesn't disturb an unexpected value") {
            auto newun = un([](int i) { return i * i; });

            CHECK(static_cast<bool>(newun) == static_cast<bool>(un));
            CHECK(newun.error() == un.error());
        }
    }

    SECTION("OOP style access") {
        SECTION("Fallback access function returns correct value if expected is contained") {
            CHECK(exp.value_or(655) == exp.value());
        }

        SECTION("Fallback access function does not throw") {
            REQUIRE_NOTHROW(un.value_or(5));
        }

        SECTION("Fallback access function returns given value if unexpected is contained") {
            REQUIRE(un.value_or(586) == 586);
        }

        SECTION("Access function throws if unexpected contained") {
            CHECK_THROWS_MATCHES(
                   (void) un.value(),
                   int,
                   Catch::Predicate<int>([](int i) {
                     return i == 42;
                   })
            );
        }

        SECTION("Yeet throws stored exception") {
            // note that yeeting with an expected value is UB
            CHECK_THROWS_MATCHES(
                   un.yeet(),
                   int,
                   Catch::Predicate<int>([](int i) {
                     return i == 42;
                   })
            );
        }

        SECTION("Apply function applies function on expected value") {
            auto f = [](int i) { return i * i; };

            auto newexp = exp.apply(f);

            CHECK(static_cast<bool>(newexp) == static_cast<bool>(exp));
            CHECK(*newexp == f(*exp));
        }

        SECTION("Apply function doesn't disturb an unexpected value") {
            auto newun = un.apply([](int i) { return i * i; });

            CHECK(static_cast<bool>(newun) == static_cast<bool>(un));
            CHECK(newun.error() == un.error());
        }
    }
}
