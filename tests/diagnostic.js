exports.print_diagnostics = function(clang, cc, files) {
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
          console.log("%s:%d:%d: %s", files[i], line, col, message);
        } else {
          console.log("%s:%d:%d: %s", files[i], line, col, message);
        }
        
      }
    }
  }
};
