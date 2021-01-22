/* InfoUtils project
 * Copyright (c) 2021 bodand
 * Licensed under the BSD 3-Clause license
 */

#include <catch2/catch.hpp>

#include <info/queue.hpp>

#include <atomic>
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
    std::thread p1([&q] {
        std::this_thread::sleep_for(50ms);
        q.push(42);
    });
    std::thread p2([&q] {
        std::this_thread::sleep_for(60ms);
        q.push(42);
    });

    CHECK(*q.await_pop() == 42);
    CHECK(*q.await_pop() == 42);
    q.end();
    p1.join();
    p2.join();
}

TEST_CASE("queue can be popped from multiple threads") {
    info::queue<int> q;
    std::atomic<int> r1 = 0;
    std::atomic<int> r2 = 0;
    std::thread t1([&q, &r1]() mutable {
        auto p = q.await_pop();
        if (p != nullptr) {
            r1 = *p;
        }
    });
    std::thread t2([&q, &r2]() mutable {
        auto p = q.await_pop();
        if (p != nullptr) {
            r2 = *p;
        }
    });

    q.push(42);
    q.push(42);
    t1.join();
    t2.join();

    CHECK(r1 == 42);
    CHECK(r2 == 42);
}

TEST_CASE("multiple producers and consumers can work on one queue") {
    info::queue<int> q;
    std::atomic<int> r1 = 0;
    std::atomic<int> r2 = 0;
    std::thread c1([&q, &r1]() mutable {
        auto p = q.await_pop();
        if (p != nullptr) {
            r1 = *p;
        }
    });
    std::thread c2([&q, &r2]() mutable {
        auto p = q.await_pop();
        if (p != nullptr) {
            r2 = *p;
        }
    });

    std::thread p1([&q] {
        std::this_thread::sleep_for(50ms);
        q.push(42);
    });
    std::thread p2([&q] {
        std::this_thread::sleep_for(60ms);
        q.push(42);
    });

    c1.join();
    c2.join();
    p1.join(); // these should happen immediately at this point
    p2.join();

    CHECK(r1 == 42);
    CHECK(r2 == 42);
}

TEST_CASE("waiting will return nullptr when the queue end()s") {
    std::atomic<int*> r = reinterpret_cast<int*>(0xDEADBEEF);
    // this is for testing purposes, because yes
    // DEREFERENCING `r' IN THIS FUNCTION IS A SUREFIRE WAY TO KILL THYSELF
    // DO NOT DO IT. SEEK HELP

    info::queue<int> q;
    std::thread t([&q, &r] {
        auto p = q.await_pop();
        r = p.get();
    });

    q.end();
    t.join();

    CHECK(r == nullptr);
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


TEST_CASE("queue can be ended before destruction") {
    info::queue<int> q;
    std::atomic<int*> r = reinterpret_cast<int*>(0xDEADBEEF);
    // DO NOT DEREFERENCE `r' IN THIS FUNCTION

    std::thread t([&q, &r]() {
        r = q.await_pop().get();
    });

    q.end();
    t.join();

    CHECK(r == nullptr);
}

TEST_CASE("ended queues get destructed peacefully") {
    info::queue<int> q;
    q.end();
}

TEST_CASE("ended queues get destructed peacefully when ending caused threads to die") {
    info::queue<int> q;
    std::thread t1([&q]() {
        (void) q.await_pop();
    });
    std::thread t2([&q]() {
        (void) q.await_pop();
    });

    q.end();
    t1.join();
    t2.join();
}
