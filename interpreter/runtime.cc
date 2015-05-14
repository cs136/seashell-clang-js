/**
 * Seashell's C Runtime Library (C++ bindings)
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
#include <emscripten/val.h>
#include "runtime.h"

using emscripten::val;
using llvm::GVTOP;
using llvm::PTOGV;

static llvm::GenericValue _stdin_read(const std::vector<llvm::GenericValue> &Args) {
  llvm::GenericValue result;
  std::string read = val::module_property("_RT_stdin_read")(val(Args[2].IntVal.getZExtValue())).as<std::string>();
  memcpy(GVTOP(Args[1]), read.c_str(), read.size());
  result.IntVal = llvm::APInt(32, read.size());
  return result;
}

static llvm::GenericValue _stdout_write(const std::vector<llvm::GenericValue> &Args) {
  llvm::GenericValue result;
  std::string buffer(static_cast<const char*>(GVTOP(Args[1])), Args[2].IntVal.getZExtValue());
  val::module_property("_RT_stdout_write")(val(buffer));
  result.IntVal = buffer.size();
  return result;
}

void SeashellInterpreter_Impl::resumeExternalFunction() {
  llvm::GenericValue result;
  /** call: _suspend */
  if (resume.F->getName() == "_suspend") {
    result.IntVal = llvm::APInt(32, val::module_property("_RT_extcall_result").as<int>());
  } 
  /** call: _read(fd, buf, len), on stdin [fd = 0] */
  else if (resume.F->getName() == "_read") {
    if (resume.ArgVals[0].IntVal != 0)
      result.IntVal = llvm::APInt(32, -_SS_EINVAL);
    else {
      result = _stdin_read(resume.ArgVals);
    }
  } 
  /** unknown call. */
  else {
    result.IntVal = llvm::APInt(32, -_SS_EINVAL);
  }
  popStackAndReturnValueToCaller(resume.F->getReturnType(), result);
  resume.F = nullptr;
}
llvm::GenericValue SeashellInterpreter_Impl::callExternalFunction(llvm::Function* F,
                                                                  const std::vector<llvm::GenericValue> &ArgVals) {
  llvm::GenericValue result;
  resume.F = F;
  resume.ArgVals = ArgVals;

  if (F->getName() == "_exit") {
    exitCalled(ArgVals[0]);
  }
  /** Needed for tests! */
  if (F->getName() == "memcpy") {
    result = PTOGV(memcpy(GVTOP(ArgVals[0]), GVTOP(ArgVals[1]), ArgVals[2].IntVal.getZExtValue()));  
  }
  else if (F->getName() == "_suspend") {
    resume.ArgVals = ArgVals;
    val::module_property("_RT_suspend")();
    // should never get here, but just in case.
    result.IntVal = llvm::APInt(32, -1);
  }
  else if (F->getName() == "_read") {
    if (ArgVals[0].IntVal != 0) {
      // TODO: Proper file descriptors.
      result.IntVal = llvm::APInt(32, -_SS_EINVAL);
    } else {
      result = _stdin_read(resume.ArgVals);
    }
  } else if (F->getName() == "_write") {
    if (ArgVals[0].IntVal != 1 && ArgVals[0].IntVal != 2) {
      // TODO: Proper file descriptors.
      result.IntVal = llvm::APInt(32, -_SS_EINVAL);
    } else
      result = _stdout_write(ArgVals);
  }
  else {
    result.IntVal = llvm::APInt(32, -_SS_EINVAL);
  }
  resume.F = nullptr;
  return result;
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

void SeashellInterpreter_Impl::LoadValueFromMemory(llvm::GenericValue& Result, llvm::GenericValue* Ptr, llvm::Type* Ty) {
  llvm::Interpreter::LoadValueFromMemory(Result, Ptr, Ty);
}

void SeashellInterpreter_Impl::StoreValueToMemory(const llvm::GenericValue& Val, llvm::GenericValue* Ptr, llvm::Type* Ty) {
  llvm::Interpreter::StoreValueToMemory(Val, Ptr, Ty);
}
