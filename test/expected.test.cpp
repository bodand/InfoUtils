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

// test'd header
#include <info/expected.hpp>

#ifndef USE_UNEXPECTED
#    pragma warning \
           "WARNING: expected.test.cpp Requires the macro USE_UNEXPECTED to be defined to the unexpected variant to use"
/* this is just to test custom USE_UNEXPECTED things, real code doesn't **need** it */
#    define USE_UNEXPECTED unexpected
#endif

using namespace info;

TEST_CASE("expected constructs from expected value",
          "[InfoUtils][expected]") {
    expected<int, int> exp{55};

    CHECK(*exp == 55);
}

TEST_CASE("expected can be copied from expected value",
          "[InfoUtils][expected]") {
    int a = 218;
    expected<int, int> exp = a;

    REQUIRE(a == 218);
    CHECK(*exp == 218);
}

TEST_CASE("expected can be move-assigned into from expected value",
          "[InfoUtils][expected]") {
    expected<int, int> exp = 125;

    CHECK(*exp == 125);
}

TEST_CASE("expected can be created from unexpected value",
          "[InfoUtils][expected]") {
    auto un = USE_UNEXPECTED{55};
    expected<int, int> exp{un};

    CHECK(exp.error() == 55);
}

TEST_CASE("expected can be copy-assigned from unexpected value",
          "[InfoUtils][expected]") {
    int a = 218;
    expected<int, int> exp = USE_UNEXPECTED{a};

    REQUIRE(a == 218);
    CHECK(exp.error() == 218);
}

TEST_CASE("expected cane be move-assigned from unexpected value",
          "[InfoUtils][expected]") {
    expected<int, int> exp = USE_UNEXPECTED{125};

    CHECK(exp.error() == 125);
}

TEST_CASE("expected can be copied from expected expected",
          "[InfoUtils][expected]") {
    expected<int, int> exp = 223;
    auto exp_c{exp};

    REQUIRE(*exp == 223);
    CHECK(*exp_c == 223);
}

TEST_CASE("expected can be copied from unexpected expected",
          "[InfoUtils][expected]") {
    expected<int, int> un = USE_UNEXPECTED{444};
    auto un_c{un};

    REQUIRE(un.error() == 444);
    CHECK(un_c.error() == 444);
}

TEST_CASE("expected can be moved from expected expected",
          "[InfoUtils][expected]") {
    expected<int, int> exp = 223;
    auto exp_m{std::move(exp)};

    CHECK(*exp_m == 223);
}

TEST_CASE("expected can be moved from unexpected expected",
          "[InfoUtils][expected]") {
    expected<int, int> un = USE_UNEXPECTED{444};
    auto un_m{std::move(un)};

    CHECK(un_m.error() == 444);
}

TEST_CASE("expected is default constructable if T is",
          "[InfoUtils][expected]") {
    expected<int, int> exp;
    int def{};

    REQUIRE(def == 0);
    CHECK(*exp == def);
}

struct Foo {
    int i;
};

expected<int, int> expe;
expected<Foo, int> fexp{Foo{4}}; // I'm sad
expected<int, int> un{USE_UNEXPECTED{42}};
expected<Foo, int> fun{USE_UNEXPECTED{42}};

TEST_CASE("explicit can be explicitly converted to boolean when true",
          "[InfoUtils][expected]") {
    CHECK(static_cast<bool>(expe));
}

TEST_CASE("explicit can be explicitly converted to boolean when false",
          "[InfoUtils][expected]") {
    CHECK_FALSE(static_cast<bool>(un));
}

TEST_CASE("expected can be dereference-accessed on expected value",
          "[InfoUtils][expected]") {
    CHECK(fexp->i == 4);
}

TEST_CASE("expected's dereference operator throws if unexpected",
          "[InfoUtils][expected]") {
    CHECK_THROWS_MATCHES(
           (void) *un,
           int,
           Catch::Predicate<int>([](int i) {
               return i == 42;
           }));
}

TEST_CASE("expected's dereference-access operator throws if unexpected",
          "[InfoUtils][expected]") {
    CHECK_THROWS_MATCHES(
           (void) fun->i,
           int,
           Catch::Predicate<int>([](int i) {
               return i == 42;
           }));
}

TEST_CASE("expected's apply operator applies function on expected value",
          "[InfoUtils][expected]") {
    auto f = [](int i) {
        return i * i;
    };

    auto newexp = expe(f);

    CHECK(static_cast<bool>(newexp) == static_cast<bool>(expe));
    CHECK(*newexp == f(*expe));
}

TEST_CASE("expected's apply operator doesn't disturb an unexpected value",
          "[InfoUtils][expected]") {
    auto newun = un([](int i) { return i * i; });

    CHECK(static_cast<bool>(newun) == static_cast<bool>(un));
    CHECK(newun.error() == un.error());
}

TEST_CASE("expected's value_or returns correct value if expected is contained",
          "[InfoUtils][expected]") {
    CHECK(expe.value_or(655) == expe.value());
}

TEST_CASE("expected's value_or does not throw",
          "[InfoUtils][expected]") {
    REQUIRE_NOTHROW(un.value_or(5));
}

TEST_CASE("expected's value_or returns given value if unexpected is contained",
          "[InfoUtils][expected]") {
    REQUIRE(un.value_or(586) == 586);
}

TEST_CASE("expected's value throws if unexpected contained",
          "[InfoUtils][expected]") {
    CHECK_THROWS_MATCHES(
           (void) un.value(),
           int,
           Catch::Predicate<int>([](int i) {
               return i == 42;
           }));
}

TEST_CASE("expected's yeet throws stored exception",
          "[InfoUtils][expected]") {
    // note that yeeting with an expected value is UB
    CHECK_THROWS_MATCHES(
           un.yeet(),
           int,
           Catch::Predicate<int>([](int i) {
               return i == 42;
           }));
}
