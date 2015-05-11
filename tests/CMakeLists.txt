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
project(seashell-js-tests)

# Tests.
file(GLOB_RECURSE seashell_nodejs_tests "${CMAKE_CURRENT_SOURCE_DIR}/*.*")
add_custom_target(seashell-nodejs-tests-copy ALL
  COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}"
  SOURCES ${seashell_nodejs_tests})
add_custom_target(link-mem-files ALL
  COMMAND ln -sf ../interpreter/seashell-runner.js.mem "${CMAKE_CURRENT_BINARY_DIR}"
  #  COMMAND ln -sf ../compiler/seashell-clang.data "${CMAKE_CURRENT_BINARY_DIR}"
  COMMAND ln -sf ../compiler/seashell-clang.js.mem "${CMAKE_CURRENT_BINARY_DIR}")

# Find node.js 
find_program(node NAMES nodejs node iojs)

add_test(NAME test1 COMMAND ${node} "${CMAKE_CURRENT_BINARY_DIR}/test1.js")
add_test(NAME test2 COMMAND ${node} "${CMAKE_CURRENT_BINARY_DIR}/test2.js") 
add_test(NAME test3 COMMAND ${node} "${CMAKE_CURRENT_BINARY_DIR}/test3.js") 
add_test(NAME test4 COMMAND ${node} "${CMAKE_CURRENT_BINARY_DIR}/test4.js") 
add_test(NAME test5 COMMAND ${node} "${CMAKE_CURRENT_BINARY_DIR}/test5.js") 
add_test(NAME test6 COMMAND ${node} "${CMAKE_CURRENT_BINARY_DIR}/test6.js") 

