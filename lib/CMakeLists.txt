# Seashell
# Copyright (C) 2015 The Seashell Maintainers
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
cmake_minimum_required(VERSION 2.8)

# Implementor's note - this file will need to be updated when changing
# versions of a library, due to changes in build system, etc.
# Contact someone at seashell@cs.uwaterloo.ca if you're uncertain about what to do
# in this case.

# It's not terribly important to keep LLVM/Clang up to date, but do so
# anyways as it's easier to make incremental changes to the build system.
# We're on 3.4 as of January '14.  Expect 3.5 before the end of the year.
# (At any rate, we should try to keep in sync with the version of clang used in RunC)

# Build our dependencies.
# External projects (CMake based)
# Global overrides 
# set(CMAKE_BUILD_TYPE "MinSizeRel")

# llvm/clang/compiler-rt (overrides).
set(LLVM_BUILD_TOOLS OFF CACHE STRING "Build LLVM tools." FORCE)
set(LLVM_ENABLE_RTTI ON CACHE STRING "Enable LLVM RTTI." FORCE)
add_subdirectory(llvm)

# Include directories.
set(LLVM_SOURCE_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/llvm/include
    PARENT_SCOPE)
set(LLVM_BINARY_INCLUDE_DIRS
    ${CMAKE_CURRENT_BINARY_DIR}/llvm/include
    PARENT_SCOPE)
set(CLANG_SOURCE_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/llvm/tools/clang/include
    PARENT_SCOPE)
set(CLANG_BINARY_INCLUDE_DIRS
    ${CMAKE_CURRENT_BINARY_DIR}/llvm/tools/clang/include
    PARENT_SCOPE)
# Set LLVM CXX Flags
set(LLVM_CXX_COMPILE_FLAGS
    ""
    PARENT_SCOPE)
