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
#include <info/nullable.hpp>

using namespace info;

TEST_CASE("nullable accepts nullptr",
          "[InfoUtils][nullable][nullability]") {
    nullable<int*> ptr{nullptr};

    CHECK(ptr.get() == nullptr);
}

TEST_CASE("nullable accepts C-NULL",
          "[InfoUtils][nullable][nullability]") {
    nullable<int*> ptr{NULL};

    CHECK(ptr.get() == nullptr);
}

TEST_CASE("nullable accepts default constructed shared_ptr smart pointer",
          "[InfoUtils][nullable][nullability]") {
    std::shared_ptr<int> ip;
    nullable<int*> ptr{ip};

    CHECK(ptr == nullptr);
}

TEST_CASE("nullable accepts valid pointer",
          "[InfoUtils][nullable][nullability]") {
    int i = 42;
    nullable ptr{&i};

    CHECK(ptr == &i);
}

TEST_CASE("nullable accepts smart_ptr smart pointer",
          "[InfoUtils][nullable][nullability]") {
    auto ip = std::make_shared<int>(42);
    nullable<int*> ptr{ip};

    CHECK(ptr == ip.get());
}

TEST_CASE("nullable converts to true when contains valid pointer",
          "[InfoUtils][nullable][nullability]") {
    int i = 42;
    nullable ptr{&i};

    CHECK(ptr);
}

TEST_CASE("nullable converts to false when contains nullptr",
          "[InfoUtils][nullable][nullability]") {
    nullable<int*> ptr{nullptr};

    CHECK_FALSE(ptr);
}

TEST_CASE("nullable does not modify pointed at value",
          "[InfoUtils][nullable][nullability]") {
    int i = 42;
    nullable ptr{&i};

    CHECK(*ptr == 42);
}

TEST_CASE("nullable allows pointer dereference-access",
          "[InfoUtils][nullable][nullability]") {
    struct Foo {
        int a;
    } foo{42};
    nullable ptr{&foo};

    CHECK(ptr->a == 42);
}

TEST_CASE("nullable can be passed from raw-pointer",
          "[InfoUtils][nullable][nullability]") {
    auto f = [](nullable<int*> ptr) {
        return ptr.get();
    };
    int i = 42;

    CHECK(*f(&i) == i);
}

TEST_CASE("nullable does not break inheritance-based polymorphism",
          "[InfoUtils][nullable][nullability]") {
    struct Foo {
        virtual int make() = 0;
        virtual ~Foo() = default;
    };

    struct Bar : Foo {
    private:
        int
        make() override {
            return 1;
        }
    } bar;

    auto f = [](nullable<Foo*> ptr) {
        return ptr->make();
    };

    CHECK(f(&bar) == 1);
}

TEST_CASE("std::hash differentiates nullables as pointers",
          "[InfoUtils][nullable][nullability]") {
    std::hash<nullable<int*>> h{};
    int a = 4, b = 2;
    nullable ptra{&a}, ptrb{&b};

    CHECK(h(ptra) != h(ptrb));
}

TEST_CASE("std::hash of nullable is deterministic",
          "[InfoUtils][nullable][nullability]") {
    std::hash<nullable<int*>> h{};
    int a = 42;
    nullable ap{&a};

    CHECK(h(ap) == h(ap));
}

TEST_CASE("nullable's nullptr hashes are equal",
          "[InfoUtils][nullable][nullability]") {
    std::hash<nullable<int*>> h{};
    nullable<int*> ptra{nullptr};
    nullable<int*> ptrb{nullptr};

    CHECK(h(ptra) == h(ptrb));
    CHECK(h(ptra) == h(nullptr));
}
