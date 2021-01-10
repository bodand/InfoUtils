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

#pragma once

// stdlib
#include <memory>
#include <type_traits>
#include <utility>

// project
#include "fail.hpp"

namespace info {
    namespace impl {
        template<class R, class... ArgsT>
        struct any_functor {
            virtual R operator()(ArgsT...) = 0;

            virtual ~any_functor() = default;
        };

        template<class Functor, class R, class... ArgsT>
        struct functor_wrapper : any_functor<R, ArgsT...> {
            static_assert(std::is_invocable_r_v<R, Functor, ArgsT...>,
                          "Supplied functor is not callable with required types.");

            R operator()(ArgsT...) noexcept(std::is_nothrow_invocable_v<Functor, ArgsT...>) override;

            functor_wrapper(Functor&& functor) noexcept(std::is_nothrow_move_constructible_v<Functor>);

        private:
            Functor _functor;
        };

        template<class Functor, class R, class... ArgsT>
        R
        functor_wrapper<Functor, R, ArgsT...>::operator()(ArgsT... args) noexcept(
               std::is_nothrow_invocable_v<Functor, ArgsT...>) {
            return _functor(args...);
        }

        template<class Functor, class R, class... ArgsT>
        functor_wrapper<Functor, R, ArgsT...>::functor_wrapper(Functor&& functor) noexcept(
               std::is_nothrow_move_constructible_v<Functor>)
             : _functor{functor} { }
    }

    template<class Functor>
    struct functor {
        static_assert(fail<Functor>::type,
                      "Stateful-functor template arguments do not match a function type of R(Args...)");
    };

    template<class R, class... ArgsT>
    struct functor<R(ArgsT...)> {
        template<class Functor>
        functor(Functor&&) noexcept;

        functor(functor&&) noexcept = default;
        functor& operator=(functor&&) noexcept = default;

        template<class... ArgsT_>
        R operator()(ArgsT_&&...);

    private:
        std::unique_ptr<impl::any_functor<R, ArgsT...>> _functor;
    };

    template<class R, class... ArgsT>
    template<class Functor>
    functor<R(ArgsT...)>::functor(Functor&& fun) noexcept
         : _functor{static_cast<impl::any_functor<R, ArgsT...>*>(
                new impl::functor_wrapper<Functor, R, ArgsT...>(std::forward<Functor>(fun)))} {
        static_assert(std::is_invocable_r_v<R, Functor, ArgsT...>,
                      "Passed functor is not invocable with required arguments and/or return type.");
    }

    template<class R, class... ArgsT>
    template<class... ArgsT_>
    R
    functor<R(ArgsT...)>::operator()(ArgsT_&&... args) {
        static_assert(sizeof...(ArgsT) == sizeof...(ArgsT_),
                      "Given argument amount differs from required amount.");
        static_assert(std::is_invocable_v<R(ArgsT...), ArgsT_...>,
                      "Given arguments are not invocable with stored functor.");

        return _functor->operator()(std::forward<ArgsT>(args)...);
    }
}
