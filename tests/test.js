var clang = require('../compiler/seashell-clang.js');
var runner = require('../interpreter/seashell-runner.js');
var diag = require('./diagnostic.js');
var fs = require('fs');
var stdout = "";
var runtime_buffer = fs.readFileSync('../compiler/crt/seashell-crt.bc');
var runtime = runtime_buffer.toString('binary');
runner._RT_stdout_write = function (string) {
  stdout += string;
  console.log(string);
};
function compile(test, source) {
  console.log("Compiling %s!", source);
  var cc = clang.seashell_compiler_make();
  clang.seashell_compiler_add_file(cc, '/working/' + source);
  test.equal(clang.seashell_compiler_run(cc), 0);
  diag.print_diagnostics(clang, cc, [source]);
  var result = clang.seashell_compiler_get_object(cc);
  clang.seashell_compiler_free(cc);
  return result;
}

exports.interpretGroup = {
  setUp: function (cb) {stdout = ""; cb();},
  
  /** Tests basic Hello World */
  testHelloWorld: function (test) {
    // Compile
    var result = compile(test, 'test-hello-world.c');

    // Interpret
    var run = new runner.SeashellInterpreter();
    test.ok(run.assemble(result));
    test.ok(run.assemble(runtime));
    test.equal(run.run(), false);
    test.equal(run.result(), 0);
    test.equal(stdout, "Hello World!\n");
    run.delete();
    
    // Finish
    test.done();
  },
  
  /** Tests basic printf */
  testPrintf: function (test) {
    // Compile
    var result = compile(test, 'test-printf.c');

    // Interpret
    var run = new runner.SeashellInterpreter();
    test.ok(run.assemble(result));
    test.ok(run.assemble(runtime));
    test.equal(run.run(), false);
    test.equal(run.result(), 0);
    test.equal(stdout, "Hello World! Foo 10 A 5.000000\n");
    run.delete();
    
    // Finish
    test.done();
  },

  /** Tests basic suspend. */
  testSuspend: function (test) {
    // Compile
    var result = compile(test, 'test-suspend.c');

    // Interpret
    var run = new runner.SeashellInterpreter();
    test.ok(run.assemble(result));
    test.ok(run.assemble(runtime));
    test.equal(run.run(), true);
    test.equal(runner._RT_resume_next, "_seashell_RT_suspend");
    test.equal(run.run(), false);
    test.equal(run.result(), 5);
    test.equal(stdout, "Hello World 5!\n");
    run.delete();

    // Finish
    test.done();
  },
  
  /** Tests Exit Success. **/
  testExitSuccess: function (test){
    //Compile
    var result = compile(test, 'test-exit-success.c');

    // Interpret
    var run = new runner.SeashellInterpreter();
    test.ok(run.assemble(result));
    test.ok(run.assemble(runtime));
    test.equal(run.run(), false);
    test.equal(run.result(), 0);
    test.equal(stdout, "Hello World!\n");
    run.delete();

    // Finish
    test.done();
  },

  /** Tests Exit Failure. **/
  testExitFailure: function (test){
    //Compile
    var result = compile(test, 'test-exit-failure.c');

    // Interpret
    var run = new runner.SeashellInterpreter();
    test.ok(run.assemble(result));
    test.ok(run.assemble(runtime));
    test.equal(run.run(), false);
    test.equal(run.result(), 55);
    test.equal(stdout, "Hello World!\n");
    run.delete();

    // Finish
    test.done();
  },
  
  /** Test Pointers */
  testPointers: function(test){
    //Compile
    var result = compile(test, 'test-pointers.c');

    // Interpret
    var run = new runner.SeashellInterpreter();
    test.ok(run.assemble(result));
    test.ok(run.assemble(runtime));
    test.equal(run.run(), false);
    test.equal(run.result(), 0);
    test.equal(stdout, "qv: 20\nmv: 20\n1\n");
    run.delete();

    // Finish
    test.done();
  },
  
  /** Test Assert */
  testAssert: function(test){
    //Compile
    var result = compile(test, 'test-assert.c');

    // Interpret
    var run = new runner.SeashellInterpreter();
    test.ok(run.assemble(result));
    test.ok(run.assemble(runtime));
    test.equal(run.run(), false);
    test.equal(run.result(), 0);
    test.equal(stdout, "Assert Passed\n");
    run.delete();

    // Finish
    test.done();
  },

  /** Test Rand 
  testRand: function(test){
    //Compile
    var result = compile(test, 'test-rand.c');

    // Interpret
    var run = new runner.SeashellInterpreter();
    test.ok(run.assemble(result));
    test.ok(run.assemble(runtime));
    test.equal(run.run(), false);
    test.equal(run.result(), 0);
    test.equal(stdout, "4\n");
    run.delete();

    // Finish
    test.done();
  },**/

  /** Test Control Flow **/
  testControlFlow: function(test){
    //Compile
    var result = compile(test, 'test-control-flow.c');
    
    //Interpret
    var run = new runner.SeashellInterpreter();
    test.ok(run.assemble(result));
    test.ok(run.assemble(runtime));
    test.equal(run.run(), false);
    test.equal(run.result(), 0);
    test.equal(stdout, "9 21 0 0\n");
    run.delete();

    //Finish
    test.done();  
  },
  /** Testing String.h functions **/
  testStrings: function(test){
    //Compile
    var result = compile(test, 'test-strings.c');
    
    //Interpret
    var run = new runner.SeashellInterpreter();
    test.ok(run.assemble(result));
    test.ok(run.assemble(runtime));
    test.equal(run.run(), false);
    test.equal(run.result(), 0);
    test.equal(stdout, "01Hello5\n");
    run.delete();

    //Finish
    test.done();  
  },
  /** Testing Memory functions **/
  testMemory: function(test){
    //Compile
    var result = compile(test, 'test-memory.c');
    
    //Interpret
    var run = new runner.SeashellInterpreter();
    test.ok(run.assemble(result));
    test.ok(run.assemble(runtime));
    test.equal(run.run(), false);
    test.equal(run.result(), 0);
    test.equal(stdout, "1234999999\n");
    run.delete();

    //Finish
    test.done();  
  }
};

