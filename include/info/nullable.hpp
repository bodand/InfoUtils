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
#include "require.hpp"
#include "_macros.hpp"

#ifdef __clang__
#  define INFO_NULLABLE _Nullable
#else
#  define INFO_NULLABLE
#endif

namespace info {
  template<class T>
  struct nullable {
      static_assert(std::is_pointer_v<T>, "Nullable type must be a pointer.");

      using pointer_type = INFO_NULLABLE T; ///< The type of the pointer stored
      using value_type = std::remove_pointer_t<T>; ///< The type the pointer points to
      using reference_type = value_type&; ///< The reference type to the pointee value
      using const_reference_type = value_type const&; ///< The const reference type to the pointee value

      nullable(pointer_type) noexcept;

      template<template<class...> class SmartPointer>
      nullable(SmartPointer<value_type>) noexcept;

      INFO_NODISCARD("Accessor")
      pointer_type get() const noexcept;

      explicit operator const bool() const noexcept;

      operator pointer_type() noexcept;
  private:
      pointer_type _ptr; ///< The stored pointer
  };

  template<class T, class U>
  bool operator==(nullable<T*>, U) noexcept;
  template<class T>
  bool operator==(nullable<T*>, bool) noexcept;
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
info::nullable<T>::operator const bool() const noexcept {
    return _ptr != nullptr;
}

template<class T, class U>
bool info::operator==(nullable<T*> nullable, U rhs) noexcept {
    if constexpr (std::is_pointer_v<U> || std::is_null_pointer_v<U>)
        return nullable.get() == rhs;
    else if constexpr (std::is_same_v<typename info::nullable<T*>::value_type, U>) {
        if (nullable) {
            return *nullable.get() == rhs;
        }
    }
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

template<class T>
bool info::operator==(info::nullable<T*> n, bool b) noexcept {
    return static_cast<bool>(n) == b;
}

