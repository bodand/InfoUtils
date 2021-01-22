/* InfoUtils project
 * Copyright (c) 2021 bodand
 * Licensed under the BSD 3-Clause license
 */
#pragma once

#include <cassert>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <atomic>

#include <info/_macros.hpp>

namespace info {
    template<class T>
    struct queue {
        using value_type = T;
        static_assert(std::is_move_constructible_v<value_type>,
                      "queue<T>: T must be move constructible");

        template<class... Args>
        void
        push(Args&&... args) {
            static_assert(std::is_constructible_v<value_type, Args...>,
                          "queue<T>::push<Args...>(): T must be constructible from Args...");

            auto nxt = std::make_unique<node>();
            auto nxt_tail = nxt.get();
            {
                std::scoped_lock lck(_m_tail);
                _tail->put_value(std::forward<Args>(args)...);
                _tail->_next = std::move(nxt);
                _tail = nxt_tail;
            }
            _cv.notify_one();
        }

        INFO_NODISCARD_JUST
        std::unique_ptr<value_type>
        try_pop() {
            auto head = pop();
            if (!head) return nullptr;
            return std::make_unique<value_type>(std::move(head->_value));
        }

        INFO_NODISCARD_JUST
        std::unique_ptr<value_type>
        await_pop() {
            auto lck = await();
            if (_end) return nullptr;
            auto head = unlocked_pop();
            if (!head) return nullptr;
            return std::make_unique<value_type>(std::move(head->_value));
        }

        void
        end() {
            if (!_end) {
                _end = true;
                _cv.notify_all();
            }
        }

        queue()
             : _head(std::make_unique<node>()),
               _tail(_head.get()),
               _end(false),
               _m_head(),
               _m_tail(),
               _cv() { }
        queue(const queue& cp) = delete;
        queue& operator=(const queue& cp) = delete;

    private:
        struct node {
            union {
                value_type _value;
            };
            bool _has_value;
            std::unique_ptr<node> _next;

            template<class... Args>
            void
            put_value(Args&&... args) {
                assert(!_has_value);

                new (&_value) value_type(std::forward<Args>(args)...);
                _has_value = true;

                assert(_has_value);
            }

            node() noexcept
                 : _has_value(false),
                   _next(nullptr) { }

            node(const node& cp) = delete;
            node& operator=(const node& cp) = delete;
            node(node&& mv) noexcept = delete;
            node& operator=(node&& mv) noexcept = delete;

            ~node() noexcept(std::is_nothrow_destructible_v<value_type>) {
                if (_has_value) _value.~value_type();
            }
        };

        std::unique_ptr<node>
        unlocked_pop() {
            if (_head.get() == tail()) return nullptr;

            auto old = std::move(_head);
            _head = std::move(old->_next);

            return old;
        }

        std::unique_ptr<node>
        pop() {
            std::scoped_lock lck(_m_head);
            return unlocked_pop();
        }

        node*
        tail() {
            std::scoped_lock lck(_m_tail);
            return _tail;
        }

        std::unique_lock<std::mutex>
        await() {
            std::unique_lock lck(_m_head);
            _cv.wait(lck, [this] { return _head.get() != tail() || _end; });
            return lck;
        }

        std::unique_ptr<node> _head;
        node* _tail;
        std::atomic<bool> _end;
        std::mutex _m_head;
        std::mutex _m_tail;
        std::condition_variable _cv;
    };
}
