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
add_subdirectory(Interpreter)

project(seashell-runner)

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
# C++ 11 mode.
set(CMAKE_CXX_FLAGS "-std=c++11 --bind --post-js ${CMAKE_CURRENT_BINARY_DIR}/../io.js ${CMAKE_CXX_FLAGS}")
set(CMAKE_C_FLAGS "--bind --post-js ${CMAKE_CURRENT_BINARY_DIR}/../io.js ${CMAKE_C_FLAGS}")

# Grab the core LLVM components.
llvm_map_components_to_libnames(REQ_LLVM_LIBRARIES core support)

# Link
add_executable(seashell-runner runner.cc)
set_target_properties(seashell-runner PROPERTIES LINK_DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/../io.js")
target_link_libraries(seashell-runner ${REQ_LLVM_LIBRARIES} seashell-interpreter-lib)
