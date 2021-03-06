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

#if __has_cpp_attribute(nodiscard) >= 201907L
#    define INFO_NODISCARD(MSG) [[nodiscard(MSG)]]
#    define INFO_NODISCARD_JUST [[nodiscard]]
#elif __has_cpp_attribute(nodiscard) >= 201603L
#    define INFO_NODISCARD(...) [[nodiscard]]
#    define INFO_NODISCARD_JUST [[nodiscard]]
#else
#    define INFO_NODISCARD(...)
#    define INFO_NODISCARD_JUST
#endif

#if __has_cpp_attribute(likely) >= 201803L
#    define INFO_LIKELY [[likely]]
#else
#    define INFO_LIKELY
#endif

#if __has_cpp_attribute(unlikely) >= 201803L
#    define INFO_UNLIKELY [[unlikely]]
#else
#    define INFO_UNLIKELY
#endif

#ifdef __GNUG__
#    define INFO_LIKELY_(...) __builtin_expect(!!(__VA_ARGS__), 1)
#    define INFO_UNLIKELY_(...) __builtin_expect(!!(__VA_ARGS__), 0)
#else
#    define INFO_LIKELY_(...) (__VA_ARGS__)
#    define INFO_UNLIKELY_(...) (__VA_ARGS__)
#endif

#ifdef __cpp_constinit
/// \deprecated
#    define INFO_CONSTINIT constinit
#else
/// \deprecated
#    define INFO_CONSTINIT constexpr
#endif

#ifdef __cpp_consteval
#    define INFO_CONSTEVAL consteval
#else
#    define INFO_CONSTEVAL constexpr
#endif

#ifdef __GNUG__
#    define INFO_UNREACHABLE __builtin_unreachable()
#elif defined(_MSC_VER)
#    define INFO_UNREACHABLE __assume(false)
#else
#    define INFO_UNREACHABLE (void) 0
#endif
