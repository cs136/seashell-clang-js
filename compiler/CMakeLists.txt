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

# C++ 11 mode.
set(CMAKE_CXX_FLAGS "-std=c++0x --bind --post-js ${CMAKE_CURRENT_BINARY_DIR}/../io.js ${CMAKE_CXX_FLAGS}")
set(CMAKE_C_FLAGS "--bind --post-js ${CMAKE_CURRENT_BINARY_DIR}/../io.js ${CMAKE_C_FLAGS}")

# Grab the core LLVM components.
llvm_map_components_to_libnames(REQ_LLVM_LIBRARIES native nativecodegen)

# Link.
add_executable(seashell-clang compiler.cc)
set_target_properties(seashell-clang PROPERTIES LINK_DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/../io.js")
target_link_libraries(seashell-clang clangFrontend clangFrontendTool clangCodeGen
                              clangSerialization clangDriver
                              clangTooling clangParse clangSema clangAnalysis
                              clangEdit clangAST clangLex clangBasic
                              ${REQ_LLVM_LIBRARIES})
# Installation step.
INSTALL(TARGETS seashell-clang RUNTIME DESTINATION bin)

