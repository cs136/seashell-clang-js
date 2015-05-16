/**
 * Seashell's C Runtime Library (build tool)
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
/** Invocation: nodejs build-crt.js <path-to-compiler.js> <output.ll> (source-files) */

var fs = require('fs');
var clang = require(process.argv[2]);
var cc = clang.seashell_compiler_make();

/** Add arguments. */
var args = ['-ffreestanding', '-DINTERNAL_NEWLIB=1', '-DCOMPILING_NEWLIB=1', '-D__IEEE_LITTLE_ENDIAN=1', '-std=gnu89',
            '-Wno-empty-body', '-Wno-visiblity', '-Wno-parentheses', '-Wno-pointer-sign', '-Wno-unknown-attributes', '-Wno-macro-redefined',
            '-I/source/lib/clang/lib/Headers', '-I/source/compiler/crt/libc/include', '-I/source/compiler/crt/libm/common'];
args.forEach(function (arg) {
  clang.seashell_compiler_add_compile_flag(cc, arg);
});

var files = [];
/** Add files. */
process.argv.forEach(function (file, index) {
  if (index > 3) {
    clang.seashell_compiler_add_file(cc, file);
    files.push(file);
  }
});

var result = clang.seashell_compiler_run(cc);
if (result !== 0) {
  console.log("Could not compile CRT library!");
  for (var i = 0; i < files.length; i++ ) {
    var n = clang.seashell_compiler_get_diagnostic_count(cc, i);
    if (n > 0) {
      console.log("Warnings and errors (%d) for file %s:", n, files[i]);
      for (var k = 0; k < n; k++) {
        var line = clang.seashell_compiler_get_diagnostic_line(cc, i, k);
        var col = clang.seashell_compiler_get_diagnostic_column(cc, i, k);
        var error = clang.seashell_compiler_get_diagnostic_error(cc, i, k);
        var message = clang.seashell_compiler_get_diagnostic_message(cc, i, k);
        
        if (error) {
          console.log("%s:%d:%d: error %s", files[i], line, col, message);
        } else {
          console.log("%s:%d:%d: warning %s", files[i], line, col, message);
        }
        
      }
    }
  }
  process.exit(1);
}

var code = clang.seashell_compiler_get_object(cc);
fs.writeFileSync(process.argv[3], code);
