// Test #2: Test that Compilation works.
var assert = require('assert');

// Load the compiler
var M = require('../compiler/seashell-clang.js');
console.log('Test framework for Seashell clang compiler.');
console.log('Clang version: %s', M.seashell_clang_version());

// Make a compiler
var D = M.seashell_compiler_make();
// Attempt to compile something
M.seashell_compiler_add_file(D, "/source/tests/test8.c");
assert.equal(0, M.seashell_compiler_run(D));
// Get output.
var S = M.seashell_compiler_get_object(D);
// Free compiler.
M.seashell_compiler_free(D);

// Load the runner
var R = require('../interpreter/seashell-runner.js');
var buffer = "";
R._RT_stdout_write = function (s) {buffer = s;};
// Make a runner
var I = new R.SeashellInterpreter(S);
assert.ok(I.assemble());
assert.throws(function () {I.start();}, /SSS _stdin_read/);
R._RT_stdin_buffer = "Hello World!\n";
assert.throws(function () {I.continue_();}, /SSS EXIT/);
assert.equal(I.result(), 0);
assert.equal(buffer, "Hello World!\n");
