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
// Created by bodand on 2020-03-22.
//

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wused-but-marked-unused"
#pragma ide diagnostic ignored "MemberFunctionCanBeStaticInspection"
#pragma ide diagnostic ignored "cert-err58-cpp"
#pragma once

#include "assertion.hpp"
#include <info/nonnull.hpp>

BOOST_AUTO_TEST_SUITE(Info)
  BOOST_AUTO_TEST_SUITE(Utils)

    BOOST_AUTO_TEST_CASE(nonnull_accepts_valid_pointer) {
        int i = 42;
        info::nonnull non{&i};

        INFO_TEST_ASSERT(non, Is().EqualTo(&i));
    }

    BOOST_AUTO_TEST_CASE(nonnull_accepts_valid_std_shared_ptr) {
        auto ip = std::make_shared<int>(42);
        info::nonnull<int*> non{ip};

        INFO_TEST_ASSERT(*non, Is().EqualTo(42));
    }

    BOOST_AUTO_TEST_CASE(nonnull_returns_true_in_bool_context) {
        int i = 42;
        info::nonnull non{&i};

        INFO_TEST_ASSERT(non, IsTrue());
    }

    BOOST_AUTO_TEST_CASE(nonnull_retains_valid_pointer_values) {
        int i = 42;
        info::nonnull non{&i};

        INFO_TEST_ASSERT(*non, Is().EqualTo(42));
    }

    BOOST_AUTO_TEST_CASE(nonnull_allows_pointer_access) {
        struct Foo {
            int a;
        } foo{42};
        info::nonnull non{&foo};

        INFO_TEST_ASSERT(non->a, Is().EqualTo(42));
    }

    BOOST_AUTO_TEST_CASE(nonnunll_works_as_function_parameter) {
        auto f = [](info::nonnull<int*> ptr) {
          return ptr.get();
        };
        int i = 42;

        INFO_TEST_ASSERT(*f(&i), Is().EqualTo(42));
    }

    BOOST_AUTO_TEST_CASE(nonnull_works_with_inheritance) {
        struct Foo {
            virtual int make() = 0;
            virtual ~Foo() = default;
        };
        struct Bar : Foo {
        private:
            int make() override { return 1; }
        };
        auto f = [](info::nonnull<Foo*> ptr) {
          return ptr->make();
        };
        Bar bar;

        INFO_TEST_ASSERT(f(&bar), Is().EqualTo(1));
    }

    BOOST_AUTO_TEST_CASE(hash_works_for_nonnull) {
        std::hash<info::nonnull<int*>> h{};
        int a = 4, b = 2;
        info::nonnull nona{&a}, nonb{&b}, nona2{&a};

        INFO_TEST_ASSERT(h(nona), Is().Not().EqualTo(h(nonb)));
        INFO_TEST_ASSERT(h(nona), Is().EqualTo(h(nona2)));
    }

    BOOST_AUTO_TEST_CASE(hash_is_consistent_for_nonnull) {
        std::hash<info::nonnull<int*>> h{};
        int a = 42;
        info::nonnull ap{&a};

        INFO_TEST_ASSERT(h(ap), Is().EqualTo(h(ap)));
    }

  BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()

#pragma clang diagnostic pop
