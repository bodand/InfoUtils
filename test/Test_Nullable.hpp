//
// Created by tudom on 2020-03-21.
//

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wused-but-marked-unused"
#pragma ide diagnostic ignored "MemberFunctionCanBeStaticInspection"
#pragma ide diagnostic ignored "cert-err58-cpp"
#pragma once

#include "assertion.hpp"
#include <boost/test/included/unit_test.hpp>
#include <info/nullable.hpp>

BOOST_AUTO_TEST_SUITE(Info)
  BOOST_AUTO_TEST_SUITE(Utils)

    BOOST_AUTO_TEST_CASE(nullable_accept_nullptr) {
        info::nullable<int*> nullable{nullptr};

        INFO_TEST_ASSERT(nullable, Is().Null());
    }

    BOOST_AUTO_TEST_CASE(nullable_accept_c_null) {
        info::nullable<int*> nullable{NULL};

        INFO_TEST_ASSERT(nullable, Is().Null());
    }

    BOOST_AUTO_TEST_CASE(nullable_accept_null_std_shared_ptr) {
        std::shared_ptr<int> null;
        info::nullable<int*> nullable{null};

        INFO_TEST_ASSERT(nullable, Is().Null());
    }

    BOOST_AUTO_TEST_CASE(nullable_retains_value_if_not_null) {
        int i = 4;
        info::nullable<int*> nullable{&i};

        INFO_TEST_ASSERT(nullable, Is().Not().Null());
        INFO_TEST_ASSERT(*nullable, Is().EqualTo(4));
    }

    BOOST_AUTO_TEST_CASE(nullable_behaves_like_raw_pointer_in_boolean_context) {
        int i = 4;
        info::nullable<int*> null{nullptr};
        info::nullable<int*> notnull{&i};

        INFO_TEST_ASSERT(null, Is().False());
        INFO_TEST_ASSERT(notnull, Is().True());
    }

  BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()

#pragma clang diagnostic pop
