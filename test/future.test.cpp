// BSD 3-Clause License
//
// Copyright (c) 2021, bodand
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

#include <chrono>
#include <future>
#include <string>
#include <thread>
using namespace std::literals;

#include <info/future.hpp>

#include <catch2/catch.hpp>

TEST_CASE("simple promise-future connection works") {
    info::promise<int> p;
    auto f = p.get_future();
    std::thread([&p] {
        p.set_value(42);
    }).join();
    CHECK(f.get() == 42);
}

TEST_CASE("promise can report exceptions") {
    info::promise<int> p;
    auto f = p.get_future();
    std::thread([&p] {
        p.set_exception(std::make_exception_ptr(std::runtime_error("task failed successfully")));
    }).join();
    CHECK_THROWS_WITH(f.get(), Catch::Equals("task failed successfully"));
}

TEST_CASE("promise throws when calling get_future a second time") {
    info::promise<int> p;
    auto f = p.get_future();
    try {
        auto f2 = p.get_future();
        p.set_value(42);
    } catch (const std::future_error& fe) {
        CHECK(fe.code() == std::future_errc::future_already_retrieved);
    } catch (const std::exception& ex) {
        FAIL("unexpected exception: "s + ex.what());
    }
    p.set_value(42); // this'll throw if creating f2 didn't
}

TEST_CASE("setting promise twice throws") {
    info::promise<int> p;
    auto f = p.get_future();
    p.set_value(1);
    try {
        p.set_value(42);
    } catch (const std::future_error& fe) {
        CHECK(fe.code() == std::future_errc::promise_already_satisfied);
        return;
    } catch (const std::exception& ex) {
        FAIL("unexpected exception: "s + ex.what());
    }
    FAIL("expected exception");
}

TEST_CASE("then returns correct future") {
    info::future<int> f;
    {
        info::promise<int> p;
        f = p.get_future();
        std::thread([&p] {
            std::this_thread::sleep_for(50ms);
            p.set_value(21);
        }).join();
    }
    auto f2 = f.then([](const int& x) {
        return x * 2;
    });
    CHECK(f2.get() == 42);
}

TEST_CASE("futures can be chained multiple times") {
    info::promise<int> p;
    auto f = p.get_future()
                    .then([](int x) {
                        return x + 1;
                    })
                    .then([](int x) {
                        return x * 2;
                    });
    std::thread([&p] {
        std::this_thread::sleep_for(50ms);
        p.set_value(20);
    }).join();
    CHECK(f.get() == 42);
}

TEST_CASE("then callback can safely throw") {
    info::promise<int> p;
    auto f = p.get_future();
    std::thread([&p] {
        std::this_thread::sleep_for(50ms);
        p.set_value(21);
    }).join();
    auto f2 = f.then([](int) -> int {
        throw std::runtime_error("yes");
    });
    CHECK_THROWS_WITH(f2.get(), Catch::Equals("yes"));
}

TEST_CASE("not setting value/exception causes promise_broken on promise destruction") {
    info::future<int> f;
    {
        info::promise<int> p;
        f = p.get_future();
    }
    CHECK_THROWS_AS(f.get(), std::future_error);
}
