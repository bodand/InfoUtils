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

project(InfoUtils_Test CXX)

enable_testing()

set(INFO_UTILS_TEST_HEADERS
    Test_Tests.hpp
    Test_Lambda.hpp
    Test_Nullable.hpp
    Test_Nonnull.hpp
    )

foreach (case IN LISTS INFO_UTILS_TEST_HEADERS)
    # line-break is important there
    set(INFO_UTILS_TEST_CASES "${INFO_UTILS_TEST_CASES}
#include \"${case}\"")
endforeach ()

configure_file(
        "${PROJECT_SOURCE_DIR}/main.cpp.in"
        "${PROJECT_SOURCE_DIR}/main.cpp"
)

add_executable(iu_test main.cpp)

target_include_directories(iu_test
                           PRIVATE ${Utils_INCLUDE_DIR})

target_link_libraries(iu_test
                      ${Utils_TARGET}
                      Boost::boost
                      snowhouse
                      )

if ((CMAKE_CXX_COMPILER_ID STREQUAL "GCC")
     OR (CMAKE_CXX_COMPILER_ID STREQUAL "Clang"))
    target_compile_options(iu_test BEFORE PUBLIC -pthread)
    target_link_options(iu_test PUBLIC -lpthread)
endif ()

add_test(InfoUtilsTests
         "iu_test -v"
         )
