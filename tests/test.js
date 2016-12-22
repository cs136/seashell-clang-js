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
runner._RT_stderr_write = function (string) {
  stdout += string;
  console.log(string);
};
function compile(test) {
  var cc = clang.seashell_compiler_make();

  var args = Array.prototype.slice.call(arguments, 1);
  for(var i = 0; i < args.length; i++) {
    console.log("Compiling %s!", args[i]);
    clang.seashell_compiler_add_file(cc, '/working/' + args[i]);
  }
  test.equal(clang.seashell_compiler_run(cc, false), 0);
  diag.print_diagnostics(clang, cc);
  var result = clang.seashell_compiler_get_object(cc);
  clang.seashell_compiler_free(cc);
  fs.writeFileSync(args[0]+'.bc', new Buffer(result, 'binary'));
  return result;
}

exports.interpretGroup = {
  setUp: function (cb) {stdout = ""; cb();},
  
  /** Tests basic Hello World **/
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
  
  /** Tests basic printf **/
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

  /** Tests basic suspend. **/
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

  /** Tests Exit Failure.**/
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
  
  /** Test Pointers **/
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
  
  /** Test Assert **/
  testAssert: function(test){
    //Compile
    var result = compile(test, 'test-assert.c');

    // Interpret
    var run = new runner.SeashellInterpreter();
    test.ok(run.assemble(result));
    test.ok(run.assemble(runtime));
    test.equal(run.run(), false);
    test.equal(run.result(), 134);
    test.equal(stdout, "Assert Passed\nassertion \"i == 3\" failed: file \"/working/test-assert.c\", line 8\n");
    run.delete();

    // Finish
    test.done();
  },

  /** Test Rand **/
  testRand: function(test){
    //Compile
    var result = compile(test, 'test-rand.c');

    // Interpret
    var run = new runner.SeashellInterpreter();
    test.ok(run.assemble(result));
    test.ok(run.assemble(runtime));
    test.equal(run.run(), false);
    test.equal(run.result(), 0);
    test.equal(stdout, "3\n"); // glibc - 4
    run.delete();

    // Finish
    test.done();
  },

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
  /* Testing Memory functions**/
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
  },

  /** Test Input  **/
  testInput: function(test) {
    var result = compile(test, 'test-input.c');
    var run = new runner.SeashellInterpreter();
    test.ok(run.assemble(result));
    test.ok(run.assemble(runtime));

    /** Blocked on input... */
    test.equal(run.run(), true);
    runner._RT_stdin_buffer = "5\n";
    /** Blocked on input... */
    test.equal(run.run(), true);
    runner._RT_stdin_buffer = "7\n";
    /** Should handle buffers automatically.*/
    test.equal(run.run(), true);
    runner._RT_stdin_buffer = "11\n13\n";
    /** Blocked on input... */
    test.equal(run.run(), true);
    /** Set the stdin buffer to null to signal EOF. */
    runner._RT_stdin_buffer = null;
    /** Program will now quit.*/
    test.equal(run.run(), false);
    test.equal(run.result(), 36);

    test.done();
  },
  /** Tests basic Absolute function **/
  testAbs: function (test) {
    // Compile
    var result = compile(test, 'test-abs.c');
    
    // Interpret
    var run = new runner.SeashellInterpreter();
    test.ok(run.assemble(result));
    test.ok(run.assemble(runtime));
    test.equal(run.run(), false);
    test.equal(run.result(), 0);
    test.equal(stdout, "10\n")
    run.delete();

    // Finish
    test.done();
  },  
  
  /** Tests Assert Failed */
  testAssertFailed: function (test) {
    // Compile
    var result = compile(test, 'test-assert-failed.c');

    // Interpret
    var run = new runner.SeashellInterpreter();
    test.ok(run.assemble(result));
    test.ok(run.assemble(runtime));
    test.equal(run.run(), false);
    test.equal(stdout, "assertion \"0\" failed: file \"/working/test-assert-failed.c\", line 3\n");
    test.equal(run.result(), 134);
    run.delete();
    
    // Finish
    test.done();
  },
  /** Tests turnary operator **/
  testTurn: function (test) {
    // Compile
    var result = compile(test, 'test-turnary.c');

    // Interpret
    var run = new runner.SeashellInterpreter();
    test.ok(run.assemble(result));
    test.ok(run.assemble(runtime));
    test.equal(run.run(), false);
    test.equal(run.result(), 0);
    test.equal(stdout, "5\n");
    run.delete();
    
    // Finish
    test.done();
  },
  /** Tests recursion **/
  testRec: function (test) {
    // Compile
    var result = compile(test, 'test-recursion.c');

    // Interpret
    var run = new runner.SeashellInterpreter();
    test.ok(run.assemble(result));
    test.ok(run.assemble(runtime));
    test.equal(run.run(), false);
    test.equal(run.result(), 0);
    test.equal(stdout, "6\n");
    run.delete();
    
    // Finish
    test.done();
  },
  /** Tests global scope **/
  testScope: function (test) {
    // Compile
    var result = compile(test, 'test-scope.c');

    // Interpret
    var run = new runner.SeashellInterpreter();
    test.ok(run.assemble(result));
    test.ok(run.assemble(runtime));
    test.equal(run.run(), false);
    test.equal(run.result(), 0);
    test.equal(stdout, "26\n");
    run.delete();
    
    // Finish
    test.done();
  },
  /** Tests forward declarations **/
  testForwardDcl: function (test) {
    // Compile
    var result = compile(test, 'test-forward.c');

    // Interpret
    var run = new runner.SeashellInterpreter();
    test.ok(run.assemble(result));
    test.ok(run.assemble(runtime));
    test.equal(run.run(), false);
    test.equal(run.result(), 0);
    test.equal(stdout, "10\n");
    run.delete();
    
    // Finish
    test.done();
  },
  /** Tests structures**/
  testStructure: function (test) {
    // Compile
    var result = compile(test, 'test-struct.c');

    // Interpret
    var run = new runner.SeashellInterpreter();
    test.ok(run.assemble(result));
    test.ok(run.assemble(runtime));
    test.equal(run.run(), false);
    test.equal(run.result(), 0);
    test.equal(stdout, "56\n");
    run.delete();
    
    // Finish
    test.done();
  },
  /** tests sizeof**/
  testsizeof: function (test) {
    // compile
    var result = compile(test, 'test-sizeof.c');

    // interpret
    var run = new runner.SeashellInterpreter();
    test.ok(run.assemble(result));
    test.ok(run.assemble(runtime));
    test.equal(run.run(), false);
    test.equal(run.result(), 0);
    test.equal(stdout, "the size of an int is: 4\nthe size of n is: 4\nthe size of a char is: 1\nthe size of c is: 1\nthe size of an int pointer is: 8\nthe size of pn is: 8\nthe size of a char pointer is: 8\nthe size of pc is: 8\n");
    run.delete();
    
    // finish
    test.done();
  },
  /** tests floats**/
  testFloat: function (test) {
    // compile
    var result = compile(test, 'test-float.c');

    // interpret
    var run = new runner.SeashellInterpreter();
    test.ok(run.assemble(result));
    test.ok(run.assemble(runtime));
    test.equal(run.run(), false);
    test.equal(run.result(), 0);
    test.equal(stdout, "the value of one dollar is: 0.999999\n");
    run.delete();
    
    // finish
    test.done();
  },
  testFloatPrint: function (test) {
    // compile
    var result = compile(test, 'test-print-float.c');

    // interpret
    var run = new runner.SeashellInterpreter();
    test.ok(run.assemble(result));
    test.ok(run.assemble(runtime));
    test.equal(run.run(), false);
    test.equal(run.result(), 0);
    test.equal(stdout, "0.100000\n");
    run.delete();
    
    // finish
    test.done();
  },

  /** tests multiple files. */
  testMultiple: function (test) {
    // Compile
    var result = compile(test, 'test-multiple-main.c', 'test-multiple-a.c');

    // Interpret
    var run = new runner.SeashellInterpreter();
    test.ok(run.assemble(result));
    test.ok(run.assemble(runtime));
    test.equal(run.run(), false);
    test.equal(run.result(), 7);
    test.equal(stdout, "Hello World!\n");
    run.delete();
    
    // Finish
    test.done();
  }

};

