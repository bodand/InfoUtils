/* InfoUtils project
 * Copyright (c) 2021 bodand
 * Licensed under the BSD 3-Clause license
 */

#include <catch2/catch.hpp>

#include <info/queue.hpp>

#include <chrono>
#include <thread>
using namespace std::literals;

TEST_CASE("queue can be pushed into") {
    info::queue<int> q;
    q.push(1);
    q.push(2);
}

TEST_CASE("empty queue can be popped from to nullptr") {
    info::queue<int> q;
    CHECK(q.try_pop() == nullptr);
}

TEST_CASE("queue can be popped from") {
    info::queue<int> q;
    q.push(42);
    CHECK(*q.try_pop() == 42);
}

TEST_CASE("queue can be pushed into from multiple threads") {
    info::queue<int> q;
    std::thread([&q] {
        std::this_thread::sleep_for(50ms);
        q.push(42);
    }).detach();
    std::thread([&q] {
        std::this_thread::sleep_for(60ms);
        q.push(42);
    }).detach();

    CHECK(*q.await_pop() == 42);
    CHECK(*q.await_pop() == 42);
}

TEST_CASE("queue can be popped from multiple threads") {
    info::queue<int> q;
    std::thread t1([&q]() mutable {
        auto p = q.await_pop();
        if (p != nullptr) {
            CHECK(*p == 42);
        } else {
            FAIL_CHECK("shit on thread#1");
        }
    });
    std::thread t2([&q]() mutable {
        auto p = q.await_pop();
        if (p != nullptr) {
            CHECK(*p == 42);
        } else {
            FAIL_CHECK("shit on thread#2");
        }
    });

    q.push(42);
    q.push(42);
    t1.join();
    t2.join();
}

TEST_CASE("multiple producers and consumers can work on one queue") {
    info::queue<int> q;
    std::thread t1([&q]() mutable {
        auto p = q.await_pop();
        if (p != nullptr) {
            CHECK(*p == 42);
        } else {
            FAIL_CHECK("shit on thread#1");
        }
    });
    std::thread t2([&q]() mutable {
        auto p = q.await_pop();
        if (p != nullptr) {
            CHECK(*p == 42);
        } else {
            FAIL_CHECK("shit on thread#2");
        }
    });

    std::thread([&q] {
        std::this_thread::sleep_for(50ms);
        q.push(42);
    }).detach();
    std::thread([&q] {
        std::this_thread::sleep_for(60ms);
        q.push(42);
    }).detach();

    t1.join();
    t2.join();
}

TEST_CASE("waiting will return nullptr when the queue dies") {
    std::thread t;
    {
        info::queue<int> q;
        t = std::thread([&q] {
            auto p = q.await_pop();
            CHECK(p == nullptr);
        });
    }

    t.join();
}

TEST_CASE("queue can handle non-copyable types") {
    info::queue<std::unique_ptr<int>> q;
    q.push(new int{42});

    static_assert(std::is_same_v<decltype(q.try_pop()), std::unique_ptr<std::unique_ptr<int>>>);
    CHECK(**q.try_pop() == 42);
}

struct throwing_foo {
    explicit throwing_foo(int) {
        throw std::runtime_error("throwing_foo");
    }
};

TEST_CASE("queue can handle a throwing constructor gracefully") {
    info::queue<throwing_foo> q;

    CHECK_THROWS_WITH(q.push(42), Catch::Equals("throwing_foo"));
    CHECK(q.try_pop() == nullptr);
}
