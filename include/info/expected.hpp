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
// Created by tudom on 2020-03-31.
//

#pragma once

// stdlib
#include <type_traits>
#include <optional>

// project
#include "_macros.hpp"

#ifndef INFO_USE_UNEXPECTED
#  ifdef _MSC_VER
#    define INFO_UNEXPECTED unexpected_
#  else
#    define INFO_UNEXPECTED unexpected
#  endif
#else
#  define INFO_UNEXPECTED INFO_USE_UNEXPECTED
#endif

namespace info {
  template<class T>
  struct INFO_UNEXPECTED {
      T value;

      explicit INFO_UNEXPECTED(const T& value) noexcept
             : value{value} {}
  };

  /**
   * \brief A exception handling type which contains the value or the reason
   * why there is no value.
   *
   * A type which stores either the value as expected from a successful computation
   * or an exception (of some type) which can be inspected as for why the computation
   * failed.
   *
   * Inspired by Andrei Alexandrescu's 'Expect the expected' talk on CppCon 2018:
   * https://www.youtube.com/watch?v=PH4WBuE1BHI
   *
   * \note If the expected value is nothing, for example a function is only called
   * for its side-effects use `expected<void, E>`.
   *
   * \tparam T The expected Type
   * \tparam E The Exception type.
   *
   * \since 1.1
   * \author bodand
   */
  template<class T, class E>
  struct expected {
      /**
       * \brief Returns whether the computation was successful.
       */
      INFO_NODISCARD("Success check always returns value")
      bool success() const noexcept;

      /**
       * \brief Returns the value, or throws if computation failed.
       *
       * Returns the value as returned by a successful computation,
       * or, if the computation happed to fail, it throws
       * the exception returned by it.
       *
       * \return The return value of the successful computation
       * \throws The exception reported by the failed computation
       */
      INFO_NODISCARD("Value accessor returns or throws")
      T value() const;

      /**
       * \brief Returns the stored value if it exists, or the supplied
       * one otherwise.
       *
       * Returns the value returned by a successful computation, or the passed
       * argument static converted to type T, when the computation has failed.
       *
       * \tparam U The type of the parameter. Mut be convertible to T.
       *
       * \return The stored value, or the passed argument depending on the
       * success of the original computation.
       */
      template<class U>
      INFO_NODISCARD("Value_or accessor always returns")
      T value_or(U&&) const noexcept(
      /**/noexcept(static_cast<T>(std::forward<U>(std::declval<U>())))
      );

      /**
       * \copydoc value
       */
      INFO_NODISCARD("Dereference operator returns value or throws")
      T operator*() const;

      /**
       * \brief Allows the returned value to be accessed through a pseudo-pointer
       * interface. Throws if computation failed.
       *
       * \return A pointer to the return value of the successful computation
       * \throws The exception reported by the failed computation
       */
      INFO_NODISCARD("Pointer access operator returns or throws")
      const T* operator->() const;

      /**
       * \brief Allows the returned value to be accessed through a pseudo-pointer
       * interface. Throws if computation failed.
       *
       * \return A pointer to the return value of the successful computation
       * \throws The exception reported by the failed computation
       */
      INFO_NODISCARD("Pointer access operator returns or throws")
      T* operator->();

      /**
       * \brief Returns the error.
       *
       * Returns the error returned by a failed computation.
       * If the computation succeeded the behavior is undefined.
       */
      INFO_NODISCARD("Error accessor returns")
      E error() const noexcept;

      /**
       * \brief Throws the contained exception.
       *
       * Throws the exception returned from a failed computation.
       * If the computation succeeded the behavior is undefined.
       */
      void yeet() const;

      /**
       * \brief Calls the provided functor on the value, if it exists. Nop otherwise.
       *
       * Applies the functor passed to the successful value if possible. If
       * not, the current exception is propagated further.
       *
       * \tparam F The type of the functor
       *
       * \return An expected which may contain the value got from the successful
       * application of the functor, or the currently contained exception.
       */
      template<class F>
      INFO_NODISCARD("Apply returns the F of the moved version of the current value")
      expected<std::invoke_result_t<F, T>, E>
      apply(F&&) const noexcept(std::is_nothrow_invocable_v<F, T>);

      /**
       * \brief Calls the provided functor on the value, if it exists. Nop otherwise.
       *
       * Applies the functor passed to the successful value if possible. If
       * not, the current exception is propagated further.
       *
       * \tparam F The type of the functor
       *
       * \return An expected which may contain the value got from the successful
       * application of the functor, or the currently contained exception.
       */
      template<class F>
      expected<std::invoke_result_t<F, T>, E>
      operator()(F&&) const noexcept(std::is_nothrow_invocable_v<F, T>);

      /**
       * \brief Returns whether the computation was successful.
       */
      explicit operator bool() const noexcept;

      template<typename = std::enable_if_t<std::is_default_constructible_v<T>>>
      expected() noexcept(std::is_nothrow_default_constructible_v<T>);

      expected(const T&) noexcept(std::is_nothrow_copy_constructible_v<T>);
      expected(T&&) noexcept(std::is_nothrow_move_constructible_v<T>);

      expected(const INFO_UNEXPECTED<E>&) noexcept(std::is_nothrow_copy_constructible_v<E>);
      expected(INFO_UNEXPECTED<E>&&) noexcept(std::is_nothrow_move_constructible_v<E>);

      expected(const expected<T, E>& cp) noexcept(
      /**/std::is_nothrow_copy_constructible_v<T>
          && std::is_nothrow_copy_constructible_v<E>
      );

      expected(expected<T, E>&& mv) noexcept(
      /**/std::is_nothrow_move_constructible_v<T>
          && std::is_nothrow_move_constructible_v<E>
      );

      template<class U,
             typename = std::enable_if_t<std::is_constructible_v<T, U>>>
      explicit expected(U&&) noexcept(std::is_nothrow_constructible_v<T, U>);

      virtual ~expected();
  private:
      union {
          T _succ; ///< Stores the successful computation's return value
          E _fail; ///< Stores the failed computation's return value
      };
      bool _ok{true}; ///< Whether or not the computation succeeded
  };

  template<class T, class E>
  bool expected<T, E>::success() const noexcept {
      return _ok;
  }

  template<class T, class E>
  T expected<T, E>::value() const {
      if (!_ok) INFO_UNLIKELY
          yeet();
      return _succ;
  }

  template<class T, class E>
  template<class U>
  T expected<T, E>::value_or(U&& other) const noexcept(
  /**/noexcept(static_cast<T>(std::forward<U>(std::declval<U>())))
  ) {
      if (_ok) INFO_LIKELY
          return _succ;
      return static_cast<T>(std::forward<T>(other));
  }

  template<class T, class E>
  T expected<T, E>::operator*() const {
      return value();
  }

  template<class T, class E>
  const T* expected<T, E>::operator->() const {
      if (_ok) INFO_LIKELY
          return &_succ;
      throw _fail;
  }

  template<class T, class E>
  T* expected<T, E>::operator->() {
      if (_ok) INFO_LIKELY
          return &_succ;
      throw _fail;
  }

  template<class T, class E>
  E expected<T, E>::error() const noexcept {
      return _fail;
  }

  template<class T, class E>
  void expected<T, E>::yeet() const {
      throw _fail;
  }

  template<class T, class E>
  template<class F>
  expected<std::invoke_result_t<F, T>, E>
  expected<T, E>::apply(F&& f) const noexcept(std::is_nothrow_invocable_v<F, T>) {
      if (_ok) INFO_LIKELY
          return std::forward<F>(f)(std::move(_succ));
      return INFO_UNEXPECTED{std::move(_fail)};
  }

  template<class T, class E>
  template<class F>
  expected<std::invoke_result_t<F, T>, E>
  expected<T, E>::operator()(F&& f) const noexcept(std::is_nothrow_invocable_v<F, T>) {
      return apply(std::forward<F>(f));
  }

  template<class T, class E>
  expected<T, E>::operator bool() const noexcept {
      return _ok;
  }

  template<class T, class E>
  template<typename>
  expected<T, E>::expected() noexcept(std::is_nothrow_default_constructible_v<T>) {
      new(&_succ) T{};
  }

  template<class T, class E>
  expected<T, E>::expected(const T& succ) noexcept(std::is_nothrow_copy_constructible_v<T>) {
      new(&_succ) T{succ};
  }

  template<class T, class E>
  expected<T, E>::expected(T&& succ) noexcept(std::is_nothrow_move_constructible_v<T>) {
      new(&_succ) T{std::move(succ)};
  }

  template<class T, class E>
  expected<T, E>::expected(const INFO_UNEXPECTED<E>& unexp) noexcept(
  /**/std::is_nothrow_copy_constructible_v<E>
  )
         : _ok{false} {
      new(&_fail) E{unexp.value};
  }

  template<class T, class E>
  expected<T, E>::expected(INFO_UNEXPECTED<E>&& un) noexcept(
  /**/std::is_nothrow_move_constructible_v<E>
  )
         : _ok(false) {
      new(&_fail) E{std::move(un.value)};
  }

  template<class T, class E>
  template<class U, typename>
  expected<T, E>::expected(U&& other) noexcept(std::is_nothrow_constructible_v<T, U>) {
      new(&_succ) T{std::forward<U>(other)};
  }

  template<class T, class E>
  expected<T, E>::expected(const expected<T, E>& cp) noexcept(
  /**/std::is_nothrow_copy_constructible_v<T>
      && std::is_nothrow_copy_constructible_v<E>
  )
         : _ok{cp._ok} {
      if (_ok) INFO_LIKELY
          new(&_succ) T{cp._succ};
      else
          new(&_fail) E{cp._fail};
  }

  template<class T, class E>
  expected<T, E>::expected(expected<T, E>&& mv) noexcept(
  /**/std::is_nothrow_move_constructible_v<T>
      && std::is_nothrow_move_constructible_v<E>
  )
         : _ok{mv._ok} {
      if (_ok) INFO_LIKELY
          new(&_succ) T{std::move(mv._succ)};
      else
          new(&_fail) E{std::move(mv._fail)};
  }

  template<class T, class E>
  expected<T, E>::~expected() {
      if (_ok) INFO_LIKELY
          _succ.~T();
      else
          _fail.~E();
  }

  // void Specialization ///////////////////////////////////////////////////////

  template<class E>
  struct expected<void, E> {
      /**
   * \brief Returns whether the computation was successful.
   */
      INFO_NODISCARD("Success check always returns value")
      bool success() const noexcept;

      /**
       * \brief Returns the error.
       *
       * Returns the error returned by a failed computation.
       * If the computation succeeded the behavior is undefined.
       */
      INFO_NODISCARD("Error accessor always returns")
      E error() const noexcept;

      /**
       * \brief Throws the contained exception.
       *
       * Throws the exception returned from a failed computation.
       * If the computation succeeded the behavior is undefined.
       */
      void yeet() const;

      /**
       * \brief Returns whether the computation was successful.
       */
      explicit operator bool() const noexcept;

      expected() noexcept;

      expected(const INFO_UNEXPECTED<E>&) noexcept(std::is_nothrow_copy_constructible_v<E>);

      expected(const expected& cp) noexcept(
      /**/std::is_nothrow_copy_constructible_v<E>
      ) = default;

      expected(expected&& mv) noexcept(
      /**/std::is_nothrow_move_constructible_v<E>
      ) = default;
  private:
      std::optional<E> _err;
  };

  template<class E>
  bool expected<void, E>::success() const noexcept {
      return (bool) _err;
  }

  template<class E>
  E expected<void, E>::error() const noexcept {
      return *_err;
  }

  template<class E>
  void expected<void, E>::yeet() const {
      throw *_err;
  }

  template<class E>
  expected<void, E>::operator bool() const noexcept {
      return (bool) _err;
  }

  template<class E>
  expected<void, E>::expected() noexcept
         : _err{std::nullopt} {}

  template<class E>
  expected<void, E>::expected(const INFO_UNEXPECTED<E>& unexp) noexcept(
  /**/std::is_nothrow_copy_constructible_v<E>
  )
         : _err{unexp.value} {}
}

#ifdef INFO_USE_UNEXPECTED
#  undef INFO_UNEXPECTED
#endif
