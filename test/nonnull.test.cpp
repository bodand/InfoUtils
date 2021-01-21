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
#include <info/nonnull.hpp>

using namespace info;

TEST_CASE("nonnull accepts valid pointer",
          "[InfoUtils][nonnull][nullability]") {
    int i = 42;
    nonnull non{&i};

    CHECK(non == &i);
}

TEST_CASE("nonnull accepts shared_ptr smart pointer",
          "[InfoUtils][nonnull][nullability]") {
    auto ip = std::make_shared<int>(42);
    info::nonnull<int*> non{ip};

    CHECK(non == ip.get());
}

TEST_CASE("nonnull always converts to true",
          "[InfoUtils][nonnull][nullability]") {
    int i = 42;
    info::nonnull non{&i};

    CHECK(non);
}

TEST_CASE("nonnull does not modify pointed at value",
          "[InfoUtils][nonnull][nullability]") {
    int i = 42;
    info::nonnull non{&i};

    CHECK(i == 42);
    CHECK(*non == 42);
}

TEST_CASE("nonnull allows dereference-access operator",
          "[InfoUtils][nonnull][nullability]") {
    struct Foo {
        int a;
    } foo{42};
    info::nonnull non{&foo};

    CHECK(non->a == 42);
}

TEST_CASE("nonnull can be passed from raw-pointer") {
    auto f = [](info::nonnull<int*> ptr) {
        return ptr.get();
    };
    int i = 42;

    CHECK(*f(&i) == i);
}

TEST_CASE("nonnull does not break inheritance-based polymorphism") {
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

    auto f = [](info::nonnull<Foo*> ptr) {
        return ptr->make();
    };

    CHECK(f(&bar) == 1);
}

TEST_CASE("std::hash differentiates nonnull as pointers") {
    std::hash<info::nonnull<int*>> h{};
    int a = 4, b = 2;
    info::nonnull nona{&a}, nonb{&b};

    CHECK(h(nona) != h(nonb));
}

TEST_CASE("std::hash on nonnull is deterministic") {
    std::hash<info::nonnull<int*>> h{};
    int a = 42;
    info::nonnull ap{&a};

    CHECK(h(ap) == h(ap));
}
