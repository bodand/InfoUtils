## BSD 3-Clause License
#
# Copyright (c) 2020, bodand
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice, this
#    list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
#
# 3. Neither the name of the copyright holder nor the names of its
#    contributors may be used to endorse or promote products derived from
#    this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

cmake_minimum_required(VERSION 3.10)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

## Project options
option(INFO_UTILS_BUILD_TESTS "Build the test-suite for InfoUtils. [On]" On)

## Project declaration
project(InfoUtils
        VERSION 1.0
        LANGUAGES CXX)

## Project settings
set(INFO_PROJECT_NAME Utils)
set(INFO_PROJECT_NAMESPACE info)

## Debug-flags
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU"
    OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_GLIBCXX_DEBUG --coverage")
endif ()

## Require C++17+
if ((NOT CMAKE_CXX_STANDARD)
     OR CMAKE_CXX_STANDARD
     AND (NOT CMAKE_CXX_STANDARD LESS "17"))
    set(CMAKE_CXX_STANDARD 17)
endif ()

## Project level macros
set(${INFO_PROJECT_NAME}_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")
set(${INFO_PROJECT_NAME}_HEADER_PATH "${${INFO_PROJECT_NAME}_INCLUDE_DIR}/info/")
set(${INFO_PROJECT_NAME}_TARGET iu)

## Initialize target
add_library(${${INFO_PROJECT_NAME}_TARGET} INTERFACE)
add_library(info::utils ALIAS ${${INFO_PROJECT_NAME}_TARGET})

# Check for required features and
#  applicable warnings
include(FeatureTests)

# Add warnings
target_compile_options(${${INFO_PROJECT_NAME}_TARGET}
                       INTERFACE
                       ${${INFO_PROJECT_NAME}_WARNINGS})

# Dependencies
include(Dependencies)

## Get dependencies
# Boost headers -> rely on system to have it
find_package(Boost REQUIRED)
target_link_libraries(${${INFO_PROJECT_NAME}_TARGET}
                      INTERFACE
                      Boost::boost # Boost
                      )

## Set target properties
target_include_directories(${${INFO_PROJECT_NAME}_TARGET}
                           INTERFACE
                           $<INSTALL_INTERFACE:include>
                           $<BUILD_INTERFACE:${${INFO_PROJECT_NAME}_INCLUDE_DIR}>
                           )


## Optionally enable tests
if (INFO_UTILS_BUILD_TESTS)
    add_subdirectory(test)
endif ()
