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
project(seashell-runner)

# Now set the header and library paths:
# Since we're working out of an out-of-tree uninstalled build of LLVM,
# we use our own calculated directories instead of the ones provided
# by LLVMConfig, which assumes that the build is installed.
include_directories( ${LLVM_BINARY_INCLUDE_DIRS} )
include_directories( ${LLVM_SOURCE_INCLUDE_DIRS} )
include_directories( ${CLANG_SOURCE_INCLUDE_DIRS} )
include_directories( ${CLANG_BINARY_INCLUDE_DIRS} )
add_definitions( ${LLVM_DEFINITIONS} -D__STDC_LIMIT_MACROS -D__STDC_CONSTANT_MACROS )

# Set the required flags
SET(CMAKE_CXX_FLAGS "--profiling -std=c++11 ${LLVM_CXX_COMPILE_FLAGS} ${CMAKE_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "--profiling -s TOTAL_MEMORY=268435456 -s DEMANGLE_SUPPORT=1 --bind --post-js ${CMAKE_CURRENT_BINARY_DIR}/../io.js --post-js ${CMAKE_CURRENT_SOURCE_DIR}/runtime.js")

# Grab the core LLVM components.
llvm_map_components_to_libnames(REQ_LLVM_LIBRARIES core support interpreter asmparser linker bitreader)

# Link
add_executable(seashell-runner seashellinterpreter.cc seashellinterpreter_impl.cc runtime.cc)
set_source_files_properties(seashellinterpreter.cc PROPERTIES OBJECT_DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/../io.js;${CMAKE_CURRENT_SOURCE_DIR}/../io.js.in;${CMAKE_CURRENT_SOURCE_DIR}/runtime.js")
target_link_libraries(seashell-runner ${REQ_LLVM_LIBRARIES})
