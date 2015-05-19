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

typedef llvm::GenericValue GV;
typedef std::vector<GV> ArgArray;
#define CHECK_FD(fd) do { \
  GV result; \
  if (fd < 0 || fd > IMPL_MAX_FDS) { \
    result.IntVal = llvm::APInt(32, -_SS_EINVAL); \
    return result; \
  } else if (fds[fd].extfd == FD_UNUSED) { \
    result.IntVal = llvm::APInt(32, -_SS_EINVAL); \
    return result; \
  } \
} while(0)

/** int32_t _seashell_RT_read(int32_t fd, void *buffer, uint32_t size). */
GV SeashellInterpreter_Impl::_RT_read(const ArgArray &Args) {
  GV result;
  result.IntVal = llvm::APInt(32, -_SS_EINVAL);
  int32_t fd = Args[0].IntVal.getSExtValue();
  void* buffer = GVTOP(Args[1]);
  uint32_t size = Args[2].IntVal.getZExtValue();
  CHECK_FD(fd);
  
  if (fds[fd].extfd == FD_INTERNAL && fds[fd].intfd == 0) {
    /** Read from stdin. */
    std::string read = val::module_property("_RT_stdin_read")(val(static_cast<uint32_t>(Args[2].IntVal.getZExtValue()))).as<std::string>();
    memcpy(GVTOP(Args[1]), read.c_str(), read.size());
    result.IntVal = llvm::APInt(32, read.size());
  } else if(fds[fd].extfd != FD_INTERNAL) {
    /** TODO: Check if buffer is valid (eventually). */
    result.IntVal = llvm::APInt(32, read(fds[fd].intfd, buffer, size));
  }
  return result;
}

/** _seashell_RT_write(int32_t fd, const void* buffer, uint33_t size). */
GV SeashellInterpreter_Impl::_RT_write(const ArgArray &Args) {
  GV result;
  result.IntVal = llvm::APInt(32, -_SS_EINVAL);
  int32_t fd = Args[0].IntVal.getSExtValue();
  const void* buffer = GVTOP(Args[1]);
  uint32_t size = Args[2].IntVal.getZExtValue();
  CHECK_FD(fd);
  
  if (fds[fd].extfd == FD_INTERNAL && (fds[fd].intfd == 1  || fds[fd].intfd == 2)) {
    /** Write to stdout, stderr. */
    GV result;
    std::string toWrite(static_cast<const char*>(buffer), size);
    val::module_property("_RT_stdout_write")(val(toWrite));
    result.IntVal = llvm::APInt(32, size);
  } else if(fds[fd].extfd != FD_INTERNAL) {
    /** TODO: Check if buffer is valid (eventually). */
    result.IntVal = llvm::APInt(32, write(fds[fd].intfd, buffer, size));
  }
  return result;
}

/** void _seashell_RT_exit(int32_t result) */
GV SeashellInterpreter_Impl::_RT_exit(const ArgArray &Args) {
  GV result;
  exitCalled(Args[0]);
  return result;
}

/** int32_t _seashell_RT_suspend(void) */
GV SeashellInterpreter_Impl::_RT_suspend(const ArgArray &Args) {
  GV result;
  val::module_property("_RT_suspend")();
  return result;
}

/** void* _seashell_RT_brk_base(void) */
GV SeashellInterpreter_Impl::_RT_brk_base(const ArgArray &Args) {
  return PTOGV(heap);
}

/** int32_t  _seashell_RT_brk(void* top) */
GV SeashellInterpreter_Impl::_RT_brk(const ArgArray& Args) {
  GV result;
  char* addr = static_cast<char*>(GVTOP(Args[0]));
  if (heap < addr && addr < heap_end) {
    result.IntVal = llvm::APInt(32, 0);
  } else {
    result.IntVal = llvm::APInt(32, -_SS_ENOMEM);
  }
  return result;
}

/** int32_t _seashell_RT_fstat(int32_t fd, int32_t *mode, uint64_t *size, int64_t *mtime, int64_t *atime, int64_t *ctime) */
GV SeashellInterpreter_Impl::_RT_fstat(const ArgArray& Args) {
  GV result;
  int32_t fd = Args[0].IntVal.getSExtValue();
  int32_t* mode = static_cast<int32_t*>(GVTOP(Args[1]));
  uint64_t* size = static_cast<uint64_t*>(GVTOP(Args[2]));
  int64_t* mtime = static_cast<int64_t*>(GVTOP(Args[3]));
  int64_t* atime = static_cast<int64_t*>(GVTOP(Args[4]));
  int64_t* ctime = static_cast<int64_t*>(GVTOP(Args[5]));
  
  result.IntVal = llvm::APInt(32, -_SS_EINVAL);
  return result; 
}
GV SeashellInterpreter_Impl::callExternalFunction(llvm::Function* F,
                                                                  const ArgArray &ArgVals) {
  GV result;
  resume.F = F;
  resume.ArgVals = ArgVals;
  
  /** Check system call table. */
  auto ifunc = ExtFuncs.find(F->getName());
  /** Function found. */
  if (ifunc != ResumeFuncs.end()) {
    result = (this->*(ifunc->second))(resume.ArgVals);
  }
  /** unknown call. */
  else {
    result.IntVal = llvm::APInt(32, -_SS_EINVAL);
  }
  
  resume.F = nullptr;
  return result;
}

/** resume: _seashell_RT_suspend */
GV SeashellInterpreter_Impl::_RT_resume_suspend(const ArgArray& Args) {
  GV result;
  result.IntVal = llvm::APInt(32, val::module_property("_RT_extcall_result").as<int>());
  return result;
}

/** resume: _seashell_RT_read */
GV SeashellInterpreter_Impl::_RT_resume_read(const ArgArray& Args) {
  return _RT_read(Args);
}

void SeashellInterpreter_Impl::resumeExternalFunction() {
  GV result;
  /** Check resume table. */
  auto ifunc = ResumeFuncs.find(resume.F->getName());
  /** Function found. */
  if (ifunc != ResumeFuncs.end()) {
    result = (this->*(ifunc->second))(resume.ArgVals);
  }
  /** unknown call. */
  else {
    result.IntVal = llvm::APInt(32, -_SS_EINVAL);
  }
  popStackAndReturnValueToCaller(resume.F->getReturnType(), result);
  resume.F = nullptr;
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
void SeashellInterpreter_Impl::exitCalled(GV V) {
  exitCalled(V.IntVal.zextOrTrunc(32).getZExtValue());
}
void SeashellInterpreter_Impl::exitCalled() {
  exitCalled(ExitValue);
}

int SeashellInterpreter_Impl::result() const {
  return result_;
}

void SeashellInterpreter_Impl::LoadValueFromMemory(GV& Result, GV* Ptr, llvm::Type* Ty) {
  llvm::Interpreter::LoadValueFromMemory(Result, Ptr, Ty);
}

void SeashellInterpreter_Impl::StoreValueToMemory(const GV& Val, GV* Ptr, llvm::Type* Ty) {
  llvm::Interpreter::StoreValueToMemory(Val, Ptr, Ty);
}
