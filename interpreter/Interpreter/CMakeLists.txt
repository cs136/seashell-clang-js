
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

# Grab the core LLVM components.
project(seashell-interpreter-lib)
llvm_map_components_to_libnames(REQ_LLVM_LIBRARIES engine)
add_library(seashell-interpreter-lib STATIC
  Execution.cpp
  ExternalFunctions.cpp
  Interpreter.cpp
  )
target_link_libraries(seashell-interpreter-lib ${REQ_LLVM_LIBRARIES})
