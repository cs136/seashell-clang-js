# Seashell
# Copyright (C) 2012-2015 The Seashell Maintainers
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# See also 'ADDITIONAL TERMS' at the end of the included LICENSE file.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
cmake_minimum_required(VERSION 2.6)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Debug' as none was specified.")
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
endif(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)

# Set Flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -s ASYNCIFY=1")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s ASYNCIFY=1")

# Build our libraries.
add_subdirectory(lib)

project(seashell-clang-js)
set(LLVM_ROOT "${CMAKE_BINARY_DIR}/lib/llvm")

# A bit of a sanity check:
if( NOT EXISTS ${LLVM_ROOT}/include/llvm )
  message(FATAL_ERROR "LLVM_ROOT (${LLVM_ROOT}) is not a valid LLVM install.  Something's gone horribly wrong.")
endif()

# We incorporate the CMake features provided by LLVM:
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${LLVM_ROOT}/share/llvm/cmake")
include(LLVMConfig)

# Now set the header and library paths:
# Since we're working out of an out-of-tree uninstalled build of LLVM,
# we use our own calculated directories instead of the ones provided
# by LLVMConfig, which assumes that the build is installed.
include_directories( ${LLVM_BINARY_INCLUDE_DIRS} )
include_directories( ${LLVM_SOURCE_INCLUDE_DIRS} )
include_directories( ${CLANG_SOURCE_INCLUDE_DIRS} )
include_directories( ${CLANG_BINARY_INCLUDE_DIRS} )
add_definitions( ${LLVM_DEFINITIONS} )

# Set the required flags
SET(CMAKE_CXX_FLAGS "${LLVM_CXX_COMPILE_FLAGS} ${CMAKE_CXX_FLAGS}")

# Configure I/O stub.
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/io.js.in" "${CMAKE_CURRENT_BINARY_DIR}/io.js")

# C++ 11 mode.
set(CMAKE_CXX_FLAGS "-std=c++0x --bind --post-js ${CMAKE_CURRENT_BINARY_DIR}/io.js ${CMAKE_CXX_FLAGS}")
set(CMAKE_C_FLAGS "--bind --post-js ${CMAKE_CURRENT_BINARY_DIR}/io.js ${CMAKE_C_FLAGS}")

# Grab the core LLVM components.
llvm_map_components_to_libnames(REQ_LLVM_LIBRARIES native nativecodegen)

# Link.
add_executable(seashell-clang compiler.cc)
set_target_properties(seashell-clang PROPERTIES LINK_DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/io.js")
target_link_libraries(seashell-clang clangFrontend clangFrontendTool clangCodeGen
                              clangSerialization clangDriver
                              clangTooling clangParse clangSema clangAnalysis
                              clangEdit clangAST clangLex clangBasic
                              ${REQ_LLVM_LIBRARIES})

# Tests.
enable_testing()
file(GLOB_RECURSE seashell_nodejs_tests "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.*")
add_custom_target(seashell-nodejs-tests-copy ALL
  COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/tests" "${CMAKE_CURRENT_BINARY_DIR}/tests"
  SOURCES ${seashell_nodejs_tests})
add_test(test1 nodejs "${CMAKE_CURRENT_BINARY_DIR}/tests/test1.js") 

# Installation step.
INSTALL(TARGETS seashell-clang RUNTIME DESTINATION bin)
