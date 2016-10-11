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
var diag = require('../../tests/diagnostic.js');

/** Add arguments. */
var args = ['-ffreestanding', '-DINTERNAL_NEWLIB=1', '-D_COMPILING_NEWLIB=1', '-D__IEEE_LITTLE_ENDIAN=1', '-std=gnu89',
            '-Wno-empty-body', '-Wno-visibility', '-Wno-parentheses', '-Wno-pointer-sign', '-Wno-unknown-attributes', '-Wno-macro-redefined',
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

var result = clang.seashell_compiler_run(cc, false);
diag.print_diagnostics(clang, cc, files);
if (result !== 0) {
  console.log("Could not compile CRT library!");
  process.exit(1);
}

var code = clang.seashell_compiler_get_object(cc);
fs.writeFileSync(process.argv[3], new Buffer(code, 'binary'));
