//
// Created by tudom on 2020-03-21.
//

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wused-but-marked-unused"
#pragma ide diagnostic ignored "MemberFunctionCanBeStaticInspection"
#pragma ide diagnostic ignored "cert-err58-cpp"
#pragma once

#include "assertion.hpp"
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

    BOOST_AUTO_TEST_CASE(nullable_allows_pointer_access_to_structure) {
        struct Foo {
            int a;
        } foo{5};
        info::nullable<Foo*> ptr{&foo};

        INFO_TEST_ASSERT(ptr, Is().Not().Null());
        INFO_TEST_ASSERT(ptr->a, Is().EqualTo(5));
    }

    BOOST_AUTO_TEST_CASE(nullable_seemlessly_works_with_pointer_parameters) {
        auto f = [](info::nullable<int*> ptr) {
          if (ptr)
              return *ptr;
          return 0;
        };
        int i = 5;

        INFO_TEST_ASSERT(f(&i), Is().EqualTo(5));
        INFO_TEST_ASSERT(f(nullptr), Is().EqualTo(0));
    }

    BOOST_AUTO_TEST_CASE(nullable_works_with_inheritence) {
        struct Foo {
            virtual int make() = 0;
            virtual ~Foo() = default;
        };
        struct Bar : Foo {
        private:
            int make() override { return 1; }
        };
        auto f = [](info::nullable<Foo*> ptr) {
          if (ptr)
              return ptr->make();
          return 0;
        };
        Bar bar;

        INFO_TEST_ASSERT(f(&bar), Is().EqualTo(1));
        INFO_TEST_ASSERT(f(nullptr), Is().EqualTo(0));
    }

  BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()

#pragma clang diagnostic pop
