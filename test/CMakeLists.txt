## BSD 3-Clause License
#
# Copyright (c) 2020-2021, bodand
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

include("${CMAKE_BINARY_DIR}/_deps/catch2-src/contrib/Catch.cmake")

set(TESTED_PROJECT_NAME ${PROJECT_NAME})
project(${TESTED_PROJECT_NAME}_Test CXX)

add_executable(${${TESTED_PROJECT_NAME}_TARGET}_test
               main.cpp
               expected.test.cpp
               fail.test.cpp
               lambda.test.cpp
               functor.test.cpp
               nonnull.test.cpp
               nullable.test.cpp
               future.test.cpp
               )

target_link_libraries(${${TESTED_PROJECT_NAME}_TARGET}_test
                      ${${TESTED_PROJECT_NAME}_NAMESPACE}
                      Catch2::Catch2
                      )

target_compile_definitions(${${TESTED_PROJECT_NAME}_TARGET}_test PRIVATE
                           -DUSE_UNEXPECTED=$<IF:$<CXX_COMPILER_ID:MSVC>,unexpected_,unexpected>)

set_target_properties(${${TESTED_PROJECT_NAME}_TARGET}_test PROPERTIES
                      CXX_STANDARD 17)
target_compile_features(${${TESTED_PROJECT_NAME}_TARGET}_test
                        PRIVATE cxx_std_17)

# Add warnings
target_compile_options(${${TESTED_PROJECT_NAME}_TARGET}_test
                       PRIVATE
                       ${${TESTED_PROJECT_NAME}_WARNINGS})

catch_discover_tests(${${TESTED_PROJECT_NAME}_TARGET}_test)
