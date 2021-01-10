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

#include <info/_macros.hpp>
#include <info/expected.hpp>
#include <info/fail.hpp>
#include <info/static_warning.hpp>

#include <cassert>
#include <condition_variable>
#include <future>
#include <mutex>
#include <thread>

namespace info {
    namespace impl {
        enum class state_status {
            InProgress,
            Completed,
            Errored
        };

        struct awaitable {
            template<class Fn>
            void
            wait(Fn&& fn) {
                std::unique_lock<std::mutex> lck(_mtx);
                _cv.wait(lck, std::forward<Fn>(fn));
            }

            template<class Rep, class Period, class Fn>
            bool
            wait_for(const std::chrono::duration<Rep, Period>& dur, Fn&& fn) {
                std::unique_lock<std::mutex> lck(_mtx);
                return _cv.wait_for(lck, dur, std::forward<Fn>(fn));
            }

            template<class Clock, class Duration, class Fn>
            bool
            wait_until(const std::chrono::time_point<Clock, Duration>& tp, Fn&& fn) {
                std::unique_lock<std::mutex> lck(_mtx);
                return _cv.wait_until(lck, tp, std::forward<Fn>(fn));
            }

            std::condition_variable _cv;

        private:
            std::mutex _mtx;
        };

        template<class S, class T>
        struct chained_state;

        template<class T>
        struct future_state : private awaitable {
            using value_type = T;
            static_warning(std::is_nothrow_destructible_v<value_type>,
                           "future_state<T>: T has a throwing destructor. "
                           "This may lead to unexpected termination.");

            template<class... Args>
            void
            put_value(Args&&... args) {
                new (&_value) value_type(std::forward<Args>(args)...);
                _status = state_status::Completed;
                _cv.notify_one();
            }
            void
            put_exception(const std::exception_ptr& ex) {
                new (&_exc) std::exception_ptr(ex);
                _status = state_status::Errored;
                _cv.notify_one();
            }

            void
            wait() {
                awaitable::wait([this] { return _status != state_status::InProgress; });
            }

            template<class Rep, class Period>
            bool
            wait_for(const std::chrono::duration<Rep, Period>& dur) {
                return awaitable::wait_for(dur, [this] { return _status != state_status::InProgress; });
            }

            template<class Clock, class Duration>
            bool
            wait_until(const std::chrono::time_point<Clock, Duration>& tp) {
                return awaitable::wait_until(tp, [this] { return _status != state_status::InProgress; });
            }

            value_type
            get() {
                wait();
                // clang-format off
                if (INFO_UNLIKELY_(_status == state_status::Errored)) INFO_UNLIKELY {
                    std::rethrow_exception(_exc);
                }
                // clang-format on
                return _value;
            }

            expected<value_type, std::exception_ptr>
            expect() {
                wait();
                // clang-format off
                if (INFO_UNLIKELY_(_status == state_status::Errored)) INFO_UNLIKELY {
                    return info::INFO_UNEXPECTED{_exc};
                }
                // clang-format on
                return _value;
            }

            future_state()
                 : awaitable(),
                   _status(state_status::InProgress) { }

            future_state(const future_state& cp) = delete;
            future_state& operator=(const future_state& cp) = delete;

            future_state(future_state&& mv) noexcept(
                   std::is_nothrow_move_constructible_v<value_type>)
                 : awaitable(),
                   _status(mv._status) {
                switch (_status) {
                case state_status::InProgress:
                    return;
                case state_status::Completed:
                    new (&_value) value_type(std::move(mv._value));
                    return;
                case state_status::Errored:
                    new (&_exc) std::exception_ptr(std::move(mv._exc));
                    return;
                }
                INFO_UNREACHABLE;
            }
            future_state&
            operator=(future_state&& mv) noexcept(
                   std::is_nothrow_move_constructible_v<value_type>) {
                _status = mv._status;
                switch (_status) {
                case state_status::InProgress:
                    break;
                case state_status::Completed:
                    new (&_value) value_type(std::move(mv._value));
                    break;
                case state_status::Errored:
                    new (&_exc) std::exception_ptr(std::move(mv._exc));
                    break;
                }
                return *this;
            }

            ~future_state() noexcept {
                switch (_status) {
                case state_status::InProgress:
                    return;
                case state_status::Completed:
                    _value.~value_type();
                    return;
                case state_status::Errored:
                    _exc.~exception_ptr();
                    return;
                }
                INFO_UNREACHABLE;
            }

        private:
            template<class S, class V>
            friend struct chained_state;

            union {
                value_type _value;
                std::exception_ptr _exc;
            };
            state_status _status;
        };

        template<class S, class T>
        struct chained_state : private awaitable {
            using value_type = T;
            using owned_type = S;
            using prev_type = typename owned_type::value_type;

            void
            wait() {
                awaitable::wait([this] { return _status != state_status::InProgress; });
            }

            template<class Rep, class Period>
            bool
            wait_for(const std::chrono::duration<Rep, Period>& dur) {
                return awaitable::wait_for(dur, [this] { return _status != state_status::InProgress; });
            }

            template<class Clock, class Duration>
            bool
            wait_until(const std::chrono::time_point<Clock, Duration>& tp) {
                return awaitable::wait_until(tp, [this] { return _status != state_status::InProgress; });
            }

            value_type
            get() {
                wait();
                // clang-format off
                if (INFO_UNLIKELY_(_status == state_status::Errored)) INFO_UNLIKELY {
                    std::rethrow_exception(_exc);
                }
                // clang-format on
                return _value;
            }

            expected<value_type, std::exception_ptr>
            expect() {
                wait();
                // clang-format off
                if (INFO_UNLIKELY_(_status == state_status::Errored)) INFO_UNLIKELY {
                    return info::INFO_UNEXPECTED{_exc};
                }
                // clang-format on
                return _value;
            }

            template<class Fn_>
            chained_state(std::unique_ptr<owned_type>&& last, Fn_&& fn)
                 : _fn(std::forward<Fn_>(fn)),
                   _status(state_status::InProgress),
                   _last_step(std::move(last)),
                   _worker([this] {
                       _last_step->wait();
                       assert(_last_step->_status != state_status::InProgress);
                       if (_last_step->_status == state_status::Errored) {
                           new (&_exc) std::exception_ptr(std::move(_last_step->_exc));
                           _status = state_status::Errored;
                       } else {
                           try {
                               new (&_value) value_type(std::move(_fn(_last_step->_value)));
                               _status = state_status::Completed;
                           } catch (...) {
                               new (&_exc) std::exception_ptr(std::current_exception());
                               _status = state_status::Errored;
                           }
                       }
                       _cv.notify_one();
                   }) { }

            chained_state(const chained_state& cp) = delete;
            chained_state& operator=(const chained_state& cp) = delete;

            chained_state(chained_state&& mv) noexcept(
                   std::is_nothrow_move_constructible_v<value_type>&&
                          std::is_nothrow_move_constructible_v<std::unique_ptr<owned_type>>)
                 : _fn(std::move(mv._fn)),
                   _status(mv._status),
                   _last_step(std::move(mv._last_step)) {
                switch (_status) {
                case state_status::InProgress:
                    return;
                case state_status::Completed:
                    new (&_value) value_type(std::move(mv._value));
                    return;
                case state_status::Errored:
                    new (&_exc) std::exception_ptr(std::move(mv._exc));
                    return;
                }
                INFO_UNREACHABLE;
            }
            chained_state&
            operator=(chained_state&& mv) noexcept(
                   std::is_nothrow_move_constructible_v<value_type>&&
                          std::is_nothrow_move_assignable_v<std::unique_ptr<owned_type>>) {
                _fn = std::move(mv._fn);
                _status = mv._status;
                _last_step = std::move(mv._last_step);
                switch (_status) {
                case state_status::InProgress:
                    break;
                case state_status::Completed:
                    new (&_value) value_type(std::move(mv._value));
                    break;
                case state_status::Errored:
                    new (&_exc) std::exception_ptr(std::move(mv._exc));
                    break;
                }
                return *this;
            }

            ~chained_state() noexcept(std::is_nothrow_destructible_v<value_type>) {
                switch (_status) {
                case state_status::InProgress: break;
                case state_status::Completed: _value.~value_type(); break;
                case state_status::Errored: _exc.~exception_ptr(); break;
                }
                _worker.join();
            }

        private:
            template<class S_, class T_>
            friend struct chained_state;

            std::function<value_type(const prev_type&)> _fn;
            union {
                value_type _value;
                std::exception_ptr _exc;
            };
            state_status _status;
            std::unique_ptr<owned_type> _last_step;
            std::thread _worker;
        };

        template<class T>
        struct promise;

        template<class S>
        struct future {
            using value_type = typename S::value_type;

            INFO_NODISCARD_JUST
            bool
            valid() const noexcept {
                return _state != nullptr;
            }

            void
            wait() {
                if (!valid()) throw std::future_error(std::future_errc::no_state);
                _state->wait();
            }

            template<class Rep, class Period>
            bool
            wait_for(const std::chrono::duration<Rep, Period>& dur) {
                if (!valid()) throw std::future_error(std::future_errc::no_state);
                return _state->wait_for(dur);
            }

            template<class Clock, class Duration>
            bool
            wait_until(const std::chrono::time_point<Clock, Duration>& tp) {
                if (!valid()) throw std::future_error(std::future_errc::no_state);
                return _state->wait_until(tp);
            }

            value_type
            get() {
                if (!valid()) throw std::future_error(std::future_errc::no_state);
                return _state->get();
            }

            expected<value_type, std::exception_ptr>
            expect() {
                if (!valid()) throw std::future_error(std::future_errc::no_state);
                return _state->expect();
            }

            template<class Fn>
            INFO_NODISCARD("After a then call the new future should be used for "
                           "all correspondence, the previous one is invalidated")
            future<chained_state<S, std::invoke_result_t<Fn, const value_type&>>> // clang-format off
            then(Fn&& fn) {
                // clang-format on
                return {std::move(_state), std::forward<Fn>(fn)};
            }

            future() noexcept
                 : _state(nullptr) { }

            future(const future& cp) = delete;
            future& operator=(const future& cp) = delete;

            future(future&& mv) noexcept
                 : _state(std::move(mv._state)) { }
            future&
            operator=(future&& mv) noexcept {
                _state = std::move(mv._state);
                return *this;
            }

        private:
            template<class T>
            friend struct promise;
            template<class S_>
            friend struct future; // we are our friend. Yes

            template<class... Args>
            future(Args&&... args)
                 : _state(std::make_unique<S>(std::forward<Args>(args)...)) { }

            explicit future(unsigned)
                 : _state(std::make_unique<S>()) {
            }

            std::unique_ptr<S> _state;
        };

        template<class T>
        struct promise {
            using value_type = T;
            using future_type = future<future_state<value_type>>;

            future_type
            get_future() {
                if (_ftr_moved) throw std::future_error(std::future_errc::future_already_retrieved);
                _ftr_moved = true;
                return std::move(_ftr);
            }

            template<class... Args>
            void
            set_value(Args&&... args) {
                if (_set) throw std::future_error(std::future_errc::promise_already_satisfied);
                _state->put_value(std::forward<Args>(args)...);
                _set = true;
            }

            void
            set_exception(const std::exception_ptr& exc) {
                if (_set) throw std::future_error(std::future_errc::promise_already_satisfied);
                _state->put_exception(exc);
                _set = true;
            }

            promise()
                 : _ftr(0xDEADBEEF),
                   _state(_ftr._state.get()),
                   _set(false),
                   _ftr_moved(false) { }

            ~promise() noexcept {
                if (!_set && _ftr_moved)
                    _state->put_exception(std::make_exception_ptr(
                           std::future_error(std::future_errc::broken_promise)));
            };

        private:
            future_type _ftr;
            future_state<value_type>* _state;
            bool _set;
            bool _ftr_moved;
        };
    }

    template<class T>
    using future = impl::future<impl::future_state<T>>;
    template<class T>
    using promise = impl::promise<T>;
}
