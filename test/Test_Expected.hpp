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
// Created by tudom on 2020-03-31.
//

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wused-but-marked-unused"
#pragma ide diagnostic ignored "MemberFunctionCanBeStaticInspection"
#pragma ide diagnostic ignored "cert-err58-cpp"
#pragma once

#include "assertion.hpp"
#include <info/expected.hpp>

BOOST_AUTO_TEST_SUITE(Info)
  BOOST_AUTO_TEST_SUITE(Utils)
    using namespace info;

    BOOST_AUTO_TEST_CASE(expected_constructs_from_expected_value) {
        expected<int, int> exp{55};

        INFO_TEST_ASSERT(*exp, Is().EqualTo(55));
    }

    BOOST_AUTO_TEST_CASE(expected_is_copy_assignable_from_expected_value) {
        int a = 218;
        expected<int, int> exp = a;

        INFO_TEST_ASSERT(a, Is().EqualTo(218));
        INFO_TEST_ASSERT(*exp, Is().EqualTo(218));
    }

    BOOST_AUTO_TEST_CASE(expexted_is_move_assignable_from_expected_value) {
        expected<int, int> exp = 204;

        INFO_TEST_ASSERT(*exp, Is().EqualTo(204));
    }

    BOOST_AUTO_TEST_CASE(expected_is_constructable_from_unexpected) {
        expected<int, int> exp{unexpected{486}};

        INFO_TEST_ASSERT(exp.error(), Is().EqualTo(486));
    }

    BOOST_AUTO_TEST_CASE(expected_is_copy_assignable_from_unexpected) {
        auto un = unexpected{348};
        expected<int, int> exp = un;

        INFO_TEST_ASSERT(exp.error(), Is().EqualTo(348));
    }

    BOOST_AUTO_TEST_CASE(expected_is_move_assignable_from_unexpected) {
        expected<int, int> exp = unexpected{940};

        INFO_TEST_ASSERT(exp.error(), Is().EqualTo(940));
    }

    BOOST_AUTO_TEST_CASE(expected_is_copiable) {
        expected<int, int> exp = 223;
        expected<int, int> un = unexpected{444};
        expected<int, int> exp_c{exp};
        expected<int, int> un_c{un};

        INFO_TEST_ASSERT(*exp_c, Is().EqualTo(223));
        INFO_TEST_ASSERT(un_c.error(), Is().EqualTo(444));
    }

    BOOST_AUTO_TEST_CASE(expected_is_movealbe) {
        expected<int, int> exp = 223;
        expected<int, int> un = unexpected{444};
        expected exp_m{std::move(exp)};
        expected un_m{std::move(un)};

        INFO_TEST_ASSERT(*exp_m, Is().EqualTo(223));
        INFO_TEST_ASSERT(un_m.error(), Is().EqualTo(444));
    }

    BOOST_AUTO_TEST_CASE(expected_is_default_constructable) {
        expected<int, int> exp;

        INFO_TEST_ASSERT(*exp, Is().EqualTo(0));
    }

    BOOST_AUTO_TEST_CASE(expected_success_returns_true_when_comp_succeeded) {
        expected<int, int> exp;

        INFO_TEST_ASSERT(exp.success(), IsTrue());
    }

    BOOST_AUTO_TEST_CASE(expected_success_returns_false_when_comp_failed) {
        expected<int, int> exp = unexpected{397};

        INFO_TEST_ASSERT(exp.success(), IsFalse());
    }

    BOOST_AUTO_TEST_CASE(expected_is_convertible_to_true_when_comp_succeeded) {
        expected<int, int> exp;

        INFO_TEST_ASSERT(static_cast<bool>(exp), IsTrue());
    }

    BOOST_AUTO_TEST_CASE(expected_is_convertible_to_false_when_comp_failed) {
        expected<int, int> exp = unexpected{248};

        INFO_TEST_ASSERT(static_cast<bool>(exp), IsFalse());
    }

    BOOST_AUTO_TEST_CASE(expected_throws_if_unexpected_is_contained_using_psuedo_ptr_interface) {
        expected<int, int> exp = unexpected{710};

        BOOST_CHECK_EXCEPTION(
               *exp,
               int,
               [](int i) {
                 return i == 710;
               }
        );
    }


    BOOST_AUTO_TEST_CASE(expected_throws_if_unexpected_is_contained_using_oo_interface) {
        expected<int, int> exp = unexpected{710};

        BOOST_CHECK_EXCEPTION(
               exp.value(),
               int,
               [](int i) {
                 return i == 710;
               }
        );
    }

    BOOST_AUTO_TEST_CASE(expected_throws_current_error_on_yeet) {
        expected<int, int> exp = unexpected{710};

        BOOST_CHECK_EXCEPTION(
               exp.yeet(),
               int,
               [](int i) {
                 return i == 710;
               }
        );
    }

    struct Foo {
        int i;
    };

    BOOST_AUTO_TEST_CASE(expected_pointer_access_throws_if_error_is_stored) {
        expected<Foo, int> exp = unexpected{78};

        BOOST_CHECK_EXCEPTION(
               exp->i,
               int,
               [](int i) {
                 return i == 78;
               }
        );
    }

    BOOST_AUTO_TEST_CASE(expected_pointer_access_returns_correct_value) {
        expected<Foo, int> exp(Foo{301}); // I'm sad

        BOOST_CHECK_NO_THROW(exp->i);
        INFO_TEST_ASSERT(exp->i, Is().EqualTo(301));
    }

    BOOST_AUTO_TEST_CASE(expected_value_or_returns_correct_value_if_success) {
        expected<int, int> exp = 56;

        INFO_TEST_ASSERT(exp.value_or(772), Is().EqualTo(56));
    }

    BOOST_AUTO_TEST_CASE(expected_value_or_returns_fallback_value_if_failure) {
        expected<int, int> exp = unexpected{56};

        BOOST_CHECK_NO_THROW(exp.value_or(323));
        INFO_TEST_ASSERT(exp.value_or(930), Is().EqualTo(930));
    }

    BOOST_AUTO_TEST_CASE(expected_apply_applies_supplied_function_if_success) {
        expected<int, int> exp = 4;

        auto ret = exp.apply([](int i) {
            return i*i;
        });

        INFO_TEST_ASSERT(ret.value(), Is().EqualTo(16));
    }

    BOOST_AUTO_TEST_CASE(expected_apply_does_not_distrub_error_value) {
        expected<int, int> exp = unexpected{4};

        BOOST_REQUIRE_EQUAL(exp.error(), 4);

        auto ret = exp.apply([](int i) { return i*i; });

        INFO_TEST_ASSERT(ret.error(), Is().EqualTo(4));
    }

    BOOST_AUTO_TEST_CASE(expected_call_operator_applies_supplied_function_if_success) {
        expected<int, int> exp = 4;

        auto ret = exp([](int i) { return i*i; });

        INFO_TEST_ASSERT(ret.value(), Is().EqualTo(16));
    }

    BOOST_AUTO_TEST_CASE(expected_call_operator_does_not_distrub_error_value) {
        expected<int, int> exp = unexpected{4};

        BOOST_REQUIRE_EQUAL(exp.error(), 4);

        auto ret = exp([](int i) { return i*i; });
        INFO_TEST_ASSERT(ret.error(), Is().EqualTo(4));
    }

  BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()

#pragma clang diagnostic pop
