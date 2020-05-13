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

// test'd
#include <info/nullable.hpp>

using namespace info;

TEST_CASE("Nullable tests", "[nullable][nullability]") {
    SECTION("Accepts nullptr") {
        nullable<int*> ptr{nullptr};

        CHECK(ptr.get() == nullptr);
    }

    SECTION("Accepts C-NULL") {
        nullable<int*> ptr{NULL};

        CHECK(ptr.get() == nullptr);
    }

    SECTION("Accepts default constructed smart pointer") {
        std::shared_ptr<int> ip;
        nullable<int*> ptr{ip};

        CHECK(ptr == nullptr);
    }

    SECTION("Accepts valid pointer") {
        int i = 42;
        nullable ptr{&i};

        CHECK(ptr == &i);
    }

    SECTION("Accepts smart pointer") {
        auto ip = std::make_shared<int>(42);
        nullable<int*> ptr{ip};

        CHECK(ptr == ip.get());
    }

    SECTION("Returns true when converted to bool") {
        int i = 42;
        nullable ptr{&i};

        CHECK(ptr);
    }

    SECTION("Does not modify pointed at value") {
        int i = 42;
        nullable ptr{&i};

        CHECK(*ptr == 42);
    }

    SECTION("Allows pointer member-access") {
        struct Foo {
            int a;
        } foo{42};
        nullable ptr{&foo};

        CHECK(ptr->a == 42);
    }

    SECTION("Can be passed as raw-pointer") {
        auto f = [](nullable<int*> ptr) {
          return ptr.get();
        };
        int i = 42;

        CHECK(*f(&i) == i);
    }

    SECTION("Does not break inheritance-based polymorphism") {
        struct Foo {
            virtual int make() = 0;
            virtual ~Foo() = default;
        };

        struct Bar : Foo {
        private:
            int make() override { return 1; }
        } bar;

        auto f = [](nullable<Foo*> ptr) {
          return ptr->make();
        };

        CHECK(f(&bar) == 1);
    }

    SECTION("Hash differentiates nullables as pointers") {
        std::hash<nullable<int*>> h{};
        int a = 4, b = 2;
        nullable ptra{&a}, ptrb{&b};

        CHECK(h(ptra) != h(ptrb));
    }

    SECTION("Hash is consistent") { // or whatever it is called
        std::hash<nullable<int*>> h{};
        int a = 42;
        nullable ap{&a};

        CHECK(h(ap) == h(ap));
    }

    SECTION("Nullptr hashes are equal") {
        std::hash<nullable<int*>> h{};
        nullable<int*> ptra{nullptr};
        nullable<int*> ptrb{nullptr};

        CHECK(h(ptra) == h(ptrb));
        CHECK(h(ptra) == h(nullptr));
    }
}
