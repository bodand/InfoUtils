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
// Created by bodand on 2020-03-22.
//

#pragma once

// stdlib
#include <type_traits>

// dep: boost
#include <boost/assert.hpp>

// project
#include "_macros.hpp"

#ifdef __clang__
#  define INFO_CLANG_NONNULL _Nonnull
#  define INFO_GCC_NONNULL __attribute__((nonnull))
#  define INFO_GCC_NONNULL_RETURN /*This causes warnings*/
#elif defined(__GNUC__)
#  define INFO_CLANG_NONNULL
#  define INFO_GCC_NONNULL __attribute__((nonnull))
#  define INFO_GCC_NONNULL_RETURN __attribute__((returns_nonnull))
#else
#  define INFO_CLANG_NONNULL
#  define INFO_GCC_NONNULL
#  define INFO_GCC_NONNULL_RETURN
#endif

namespace info {
  /**
   * \brief A pointer type which cannot be null.
   *
   * A pointer wrapper type which explicitly disallows nullability. Can be,
   * and is used by Info projects, to mark all pointer parameters which do
   * not accept null.
   *
   * __SPECIFIC__: Clang
   * Usage fulfills the `nullability-completeness` warnings, and all
   * pointer usages are marked with `_Nonnull` attribute.
   *
   * __SPECIFIC__: GCC
   * Usage tells gcc that the pointer that got returned form a nonnull
   * may not be null. Mostly for allowing some optimizations, which
   * may or may not happen anyways.
   *
   * \tparam T A pointer type
   *
   * \since 1.0
   * \author bodand
   */
  template<class T>
  struct nonnull {
      static_assert(std::is_pointer_v<T>, "Nonnull type must be a pointer.");
      static_assert(!std::is_null_pointer_v<T>, "Nonnull type must not be nullptr_t.");

      using pointer_type = INFO_CLANG_NONNULL T; ///< The type of the pointer stored
      using value_type = std::remove_pointer_t<T>; ///< The type the pointer points to
      using reference_type = value_type&; ///< The reference type to the pointee value
      using const_reference_type = value_type const&; ///< The const reference type to the pointee value

      /**
       * \brief Constructor. Creates a nonnull pointer from a raw pointer.
       */
      nonnull(pointer_type) noexcept INFO_GCC_NONNULL;

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
      nonnull(SmartPointer<value_type>) noexcept;

      /**
       * \brief Returns the pointed at value.
       * \return The pointee value
       */
      INFO_NODISCARD("Accessor")
      reference_type operator*() noexcept;
      /**
       * \brief A const reference to the pointed at value
       * \return The pointee value
       */
      INFO_NODISCARD("Accessor")
      const_reference_type operator*() const noexcept;

      /**
       * \brief A pointer access operator to the pointed at value
       * \return
       */
      INFO_NODISCARD("Accessor")
      pointer_type operator->() noexcept INFO_GCC_NONNULL_RETURN;

      /**
       * \brief Returns the stored pointer. Never null.
       * \return The value of the pointer stored.
       */
      INFO_NODISCARD("Accessor")
      pointer_type get() const noexcept INFO_GCC_NONNULL_RETURN;

      /**
       * \brief Returns whether the stored pointer is null. Therefore,
       *         always returns true.
       * \return true
       */
      explicit operator bool() const noexcept;

      /**
       * \brief Implicitly casts the nonnull object to the stored pointer.
       * \return The stored pointer.
       */
      operator pointer_type() noexcept INFO_GCC_NONNULL_RETURN;
  private:
      pointer_type _ptr; ///< The actual pointer
  };


  template<class T, class U>
  bool operator==(nonnull<T*>, U) noexcept;
  template<class U, class T>
  bool operator==(U, nonnull<T*>) noexcept;

  template<class T, class U>
  bool operator!=(nonnull<T*>, U) noexcept;
  template<class U, class T>
  bool operator!=(U, nonnull<T*>) noexcept;
}

template<class T>
info::nonnull<T>::operator bool() const noexcept {
    return true;
}

template<class T>
info::nonnull<T>::operator pointer_type() noexcept {
    return _ptr;
}

template<class T>
info::nonnull<T>::nonnull(pointer_type ptr) noexcept
       : _ptr{ptr} {
    BOOST_ASSERT_MSG(_ptr != nullptr, "Pointer assigned to nonnull must not be nullptr.");
}

template<class T>
template<template<class...> class SmartPointer>
info::nonnull<T>::nonnull(SmartPointer<value_type> ptr) noexcept
       : _ptr{ptr.get()} {
    BOOST_ASSERT_MSG(_ptr != nullptr, "Pointer assigned to nonnull must not be nullptr.");
}

template<class T>
typename info::nonnull<T>::reference_type
info::nonnull<T>::operator*() noexcept {
    return *_ptr;
}

template<class T>
typename info::nonnull<T>::const_reference_type
info::nonnull<T>::operator*() const noexcept {
    return *_ptr;
}

template<class T>
typename info::nonnull<T>::pointer_type
info::nonnull<T>::operator->() noexcept {
    return _ptr;
}

template<class T>
typename info::nonnull<T>::pointer_type
info::nonnull<T>::get() const noexcept {
    return _ptr;
}

template<class T, class U>
bool info::operator==(nonnull<T*> lhs, U rhs) noexcept {
    if constexpr (std::is_pointer_v<U> || std::is_null_pointer_v<U>)
        return lhs.get() == rhs;
    else if constexpr (std::is_same_v<bool, U>)
        return static_cast<bool>(lhs) == rhs;
    else
        return false;
}

template<class U, class T>
bool info::operator==(U lhs, info::nonnull<T*> rhs) noexcept {
    return rhs == lhs;
}

template<class T, class U>
bool info::operator!=(info::nonnull<T*> lhs, U rhs) noexcept {
    return !(lhs == rhs);
}

template<class U, class T>
bool info::operator!=(U lhs, info::nonnull<T*> rhs) noexcept {
    return rhs != lhs;
}
