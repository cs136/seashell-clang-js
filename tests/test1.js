var M = require('../compiler/seashell-clang.js');
console.log('Test framework for Seashell clang compiler.');
console.log('Clang version: %s', M.seashell_clang_version());
var D = M.seashell_compiler_make();
// Attempt to compile something
M.seashell_compiler_add_file(D, "/source/tests/test1.c");
M.seashell_compiler_run(D);
console.log(M.seashell_compiler_get_object(D));
M.seashell_compiler_free(D);
