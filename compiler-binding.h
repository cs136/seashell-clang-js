/**
 * Seashell's LLVM and Clang interface.
 * Copyright (C) 2013-2015 The Seashell Maintainers.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * See also 'ADDITIONAL TERMS' at the end of the included LICENSE file.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __COMPILER_B_H__
#define __COMPILER_B_H__
#include <emscripten/bind.h>
using namespace emscripten;
EMSCRIPTEN_BINDINGS(compiler) {
  function("seashell_clang_version", &seashell_clang_version, allow_raw_pointers());
  function("seashell_compiler_make", &seashell_compiler_make, allow_raw_pointers());
  function("seashell_compiler_free", &seashell_compiler_free, allow_raw_pointers());
  function("seashell_compiler_add_file", &seashell_compiler_add_file, allow_raw_pointers());
  function("seashell_compiler_clear_files", &seashell_compiler_clear_files, allow_raw_pointers());
  function("seashell_compiler_add_compile_flag", &seashell_compiler_add_compile_flag, allow_raw_pointers());
  function("seashell_compiler_clear_compile_flags", &seashell_compiler_clear_compile_flags, allow_raw_pointers());
  function("seashell_compiler_get_linker_messages", &seashell_compiler_get_linker_messages, allow_raw_pointers());
  function("seashell_compiler_get_diagnostic_count", &seashell_compiler_get_diagnostic_count, allow_raw_pointers());
  function("seashell_compiler_get_diagnostic_line", &seashell_compiler_get_diagnostic_line, allow_raw_pointers());
  function("seashell_compiler_get_diagnostic_column", &seashell_compiler_get_diagnostic_column, allow_raw_pointers());
  function("seashell_compiler_get_diagnostic_error", &seashell_compiler_get_diagnostic_error, allow_raw_pointers());
  function("seashell_compiler_get_diagnostic_file", &seashell_compiler_get_diagnostic_file, allow_raw_pointers());
  function("seashell_compiler_get_diagnostic_message", &seashell_compiler_get_diagnostic_message, allow_raw_pointers());
  function("seashell_compiler_run", &seashell_compiler_run, allow_raw_pointers());
  function("seashell_compiler_get_object", &seashell_compiler_get_object, allow_raw_pointers());
  function("seashell_compiler_object_arch", &seashell_compiler_object_arch, allow_raw_pointers());
  function("seashell_compiler_object_os", &seashell_compiler_object_os, allow_raw_pointers());
  class_<seashell_compiler>("seashell_compiler");
}
#endif
