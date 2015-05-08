// Test #1: Test that Compilation works.
var assert = require('assert');

// Load the compiler
var M = require('../compiler/seashell-clang.js');
console.log('Test framework for Seashell clang compiler.');
console.log('Clang version: %s', M.seashell_clang_version());

// Make a compiler
var D = M.seashell_compiler_make();
// Attempt to compile something
M.seashell_compiler_add_file(D, "/source/tests/test1.c");
assert.equal(0, M.seashell_compiler_run(D));
// Log output (just in case)
console.log(M.seashell_compiler_get_object(D));
// Free compiler.
M.seashell_compiler_free(D);
