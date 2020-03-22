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
// Created by bodand on 2020-03-21.
//

#pragma once

// stdlib
#include <type_traits>

// project
#include "_macros.hpp"

#ifdef __clang__
#  define INFO_NULLABLE _Nullable
#else
#  define INFO_NULLABLE
#endif

namespace info {
  /**
   * \brief Explicitly nullable pointer type.
   *
   * A pointer wrapper type which explicitly allows nullability. Can be,
   * and is used by Info projects, to mark all pointer parameters which accept null.
   *
   * __SPECIFIC__: Clang
   * Usage fulfills the `nullability-completeness` warnings, and all
   * pointer usages are marked with `_Nullable` attribute.
   *
   * \tparam T A pointer type
   *
   * \since 1.0
   * \author bodand
   */
  template<class T>
  struct nullable {
      static_assert(std::is_pointer_v<T>, "Nullable type must be a pointer.");

      using pointer_type = INFO_NULLABLE T; ///< The type of the pointer stored
      using value_type = std::remove_pointer_t<T>; ///< The type the pointer points to
      using reference_type = value_type&; ///< The reference type to the pointee value
      using const_reference_type = value_type const&; ///< The const reference type to the pointee value

      /**
       * \brief Constructor. Creates a nullable pointer from a raw pointer.
       */
      nullable(pointer_type) noexcept;

      /**
       * \brief Constructor. Creates a nullable pointer from any
       * smart pointer.
       *
       * Creates a nullable pointer from a smart pointer instance which
       * has the same API as the standard shared and unique pointers do.
       *
       * \tparam SmartPointer The type of smart pointer to build from. Must be
       *                       compatible with std smart pointers.
       */
      template<template<class...> class SmartPointer>
      nullable(SmartPointer<value_type>) noexcept;

      /**
       * \brief Returns the pointed at value. UB if null.
       * \return The pointee value
       */
      INFO_NODISCARD("Accessor")
      reference_type operator*();
      /**
       * \brief A const reference to the pointed at value. UB if null
       * \return The pointee value
       */
      INFO_NODISCARD("Accessor")
      const_reference_type operator*() const;

      /**
       * \brief A pointer access operator to the pointed at value. UB if null
       * \return
       */
      INFO_NODISCARD("Accessor")
      pointer_type operator->();

      /**
       * \brief Returns the stored pointer. Of course, may be null.
       * \return The value of the pointer stored.
       */
      INFO_NODISCARD("Accessor")
      pointer_type get() const noexcept;

      /**
       * \brief Returns whether the stored pointer is null.
       */
      explicit operator bool() const noexcept;

      /**
       * \brief Implicitly casts the nullable object to the stored pointer.
       * \return The stored pointer.
       */
      operator pointer_type() noexcept;
  private:
      pointer_type _ptr; ///< The stored pointer
  };

  template<class T, class U>
  bool operator==(nullable<T*>, U) noexcept;
  template<class U, class T>
  bool operator==(U, nullable<T*>) noexcept;

  template<class T, class U>
  bool operator!=(nullable<T*>, U) noexcept;
  template<class U, class T>
  bool operator!=(U, nullable<T*>) noexcept;
}

template<class T>
info::nullable<T>::nullable(pointer_type ptr) noexcept
       : _ptr{ptr} {}

template<class T>
template<template<class...> class SmartPointer>
info::nullable<T>::nullable(SmartPointer<value_type> sptr) noexcept
       : _ptr{sptr.get()} {}

template<class T>
info::nullable<T>::operator pointer_type() noexcept {
    return _ptr;
}

template<class T>
typename info::nullable<T>::pointer_type
info::nullable<T>::get() const noexcept {
    return _ptr;
}

template<class T>
info::nullable<T>::operator bool() const noexcept {
    return _ptr != nullptr;
}

template<class T>
typename info::nullable<T>::reference_type
info::nullable<T>::operator*() {
    return *_ptr;
}

template<class T>
typename info::nullable<T>::const_reference_type
info::nullable<T>::operator*() const {
    return *_ptr;
}

template<class T>
typename info::nullable<T>::pointer_type
info::nullable<T>::operator->() {
    return _ptr;
}

template<class T, class U>
bool info::operator==(nullable<T*> nullable, U rhs) noexcept {
    if constexpr (std::is_pointer_v<U> || std::is_null_pointer_v<U>)
        return nullable.get() == rhs;
    else if constexpr (std::is_same_v<bool, U>)
        return static_cast<bool>(nullable) == rhs;
    else
        return false;
}

template<class T, class U>
bool info::operator!=(nullable<T*> nullable, U rhs) noexcept {
    return !(nullable == rhs);
}

template<class U, class T>
bool info::operator==(U lhs, info::nullable<T*> rhs) noexcept {
    return rhs == lhs;
}

template<class U, class T>
bool info::operator!=(U lhs, info::nullable<T*> rhs) noexcept {
    return rhs != lhs;
}

