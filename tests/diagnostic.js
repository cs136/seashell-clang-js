exports.print_diagnostics = function(clang, cc) {
  var n = clang.seashell_compiler_get_diagnostic_count(cc);
  if(n > 0) {
    console.log("Warnings and errors (%d):", n);
    for(var k = 0; k < n; k++) {
      var line = clang.seashell_compiler_get_diagnostic_line(cc, k);
      var col = clang.seashell_compiler_get_diagnostic_column(cc, k);
      var file = clang.seashell_compiler_get_diagnostic_file(cc, k);
      var error = clang.seashell_compiler_get_diagnostic_error(cc, k);
      var message = clang.seashell_compiler_get_diagnostic_message(cc, k);
      
      if (error) {
        console.log("%s:%d:%d: %s", file, line, col, message);
      } else {
        console.log("%s:%d:%d: %s", file, line, col, message);
      }
      
    }
  }
};
