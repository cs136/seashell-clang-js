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

#include "seashellinterpreter_impl.h"
#include <emscripten.h>

SeashellInterpreter_Impl::SeashellInterpreter_Impl(std::unique_ptr<llvm::Module> M)
  : llvm::Interpreter(std::move(M)), result_(-1) {
    fds[0].extfd = fds[1].extfd = fds[2].extfd = -1;
    for (int i = 3; i < IMPL_MAX_FDS; i++)
      fds[i].extfd = -2;
  }

void SeashellInterpreter_Impl::run() {
  if (resume.F) {
    resumeExternalFunction();
  }
  
  // If this function returns, then we've successfully quit. 
  llvm::Interpreter::run();
  exitCalled();
}

void SeashellInterpreter_Impl::resumeExternalFunction() {
  llvm::GenericValue result;
  fprintf(stderr, "resuming function %s\r\n", resume.F->getName().data());
  if (resume.F->getName() == "_suspend") {
    result.IntVal = llvm::APInt(32, 5);
  }
  popStackAndReturnValueToCaller(resume.F->getReturnType(), result);
  resume.F = nullptr;
}
llvm::GenericValue SeashellInterpreter_Impl::callExternalFunction(llvm::Function* F,
                                                                  const std::vector<llvm::GenericValue> &ArgVals) {
  fprintf(stderr, "suspending function %s\r\n", F->getName().data());
  if (F->getName() == "_suspend") {
    resume.F = F;
    EM_ASM(
       Runtime.stackRestore(STACK_BASE);
       throw "SSS _suspend";
    );
  } else {
    return llvm::GenericValue();
  }
}

void SeashellInterpreter_Impl::LoadValueFromMemory(llvm::GenericValue& Result, llvm::GenericValue* Ptr, llvm::Type* Ty) {
  llvm::Interpreter::LoadValueFromMemory(Result, Ptr, Ty);
}

void SeashellInterpreter_Impl::StoreValueToMemory(const llvm::GenericValue& Val, llvm::GenericValue* Ptr, llvm::Type* Ty) {
  llvm::Interpreter::StoreValueToMemory(Val, Ptr, Ty);
}

void SeashellInterpreter_Impl::start() {
  llvm::Function* main = FindFunctionNamed("main");
  std::vector<std::string> argv = {"seashell-module"};

  runStaticConstructorsDestructors(false);
  // Run main. (note: this only returns if no async calls happen)
  // In general result is stored in ExitValue.
  int result = runFunctionAsMain(main, argv, nullptr);
  exitCalled(result);
}

void SeashellInterpreter_Impl::exitCalled(int result) {
  // Clear the execution stack.
  ECStack.clear();
  runAtExitHandlers();
  // Run static destructors.
  runStaticConstructorsDestructors(true);
  // Set result
  result_ = result;
  // Toss exception to exit, reset emscripten stack.
  EM_ASM(
      Runtime.stackRestore(STACK_BASE);
      throw "SSS EXIT";
  );
}
void SeashellInterpreter_Impl::exitCalled(llvm::GenericValue GV) {
  exitCalled(GV.IntVal.zextOrTrunc(32).getZExtValue());
}
void SeashellInterpreter_Impl::exitCalled() {
  exitCalled(ExitValue);
}

int SeashellInterpreter_Impl::result() const {
  return result_;
}
