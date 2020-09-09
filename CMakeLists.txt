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
list(PREPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

## Project declaration
project(InfoUtils
        VERSION 1.5.1
        LANGUAGES CXX)

if (CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    set(${PROJECT_NAME}_MAIN On)
else ()
    set(${PROJECT_NAME}_MAIN Off)
endif ()

## Project options
option(INFO_UTILS_BUILD_TESTS "Build the test-suite for InfoUtils.
Deprecated: use ${PROJECT_NAME}_BUILD_TESTS. On is ignored and behavior is handled
by ${PROJECT_NAME}_BUILD_TESTS. Off disables tests. [On]" On)
option(${PROJECT_NAME}_BUILD_TESTS "Build the ${PROJECT_NAME} test suite. [Off as dependency]"
       ${${PROJECT_NAME}_MAIN})

if (NOT INFO_UTILS_BUILD_TESTS)
    message(WARNING "INFO_UTILS_BUILD_TESTS is deprecated. Update to use ${PROJECT_NAME}_BUILD_TESTS.
Behavior changed such that INFO_UTILS_BUILD_TESTS being set to On (default) is ignored
and ${PROJECT_NAME}_BUILD_TESTS is used to control tests. If it's set to Off tests are disabled
and ${PROJECT_NAME}_BUILD_TESTS is ignored.")
    set(${PROJECT_NAME}_BUILD_TESTS Off)
endif ()

## Debug-flags
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_GLIBCXX_DEBUG")

## Project level macros
set(${PROJECT_NAME}_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")
set(${PROJECT_NAME}_HEADER_PATH "${${PROJECT_NAME}_INCLUDE_DIR}/info/")
set(${PROJECT_NAME}_TARGET "${PROJECT_NAME}")
set(${PROJECT_NAME}_NAMESPACE info::utils)

## Initialize target
add_library(${${PROJECT_NAME}_TARGET} INTERFACE)
add_library(info::utils ALIAS ${${PROJECT_NAME}_TARGET})

## Require C++17
set_target_properties(${${PROJECT_NAME}_TARGET}
                      PROPERTIES
                      INTERFACE_CXX_STANDARD 17)
target_compile_features(${${PROJECT_NAME}_TARGET} INTERFACE
                        cxx_std_17)

## Dependencies
include(dependencies-${PROJECT_NAME})

## Warnings
include(warnings)

## Set target properties
target_include_directories(${${PROJECT_NAME}_TARGET}
                           INTERFACE
                           $<INSTALL_INTERFACE:include>
                           $<BUILD_INTERFACE:${${PROJECT_NAME}_INCLUDE_DIR}>
                           )

## Optionally enable tests
if (${PROJECT_NAME}_BUILD_TESTS)
    enable_testing()
    include(CTest)

    add_subdirectory(test)
endif ()
