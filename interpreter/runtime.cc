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
#include <stdexcept>
#include <sys/stat.h>
#include <sys/time.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

using emscripten::val;
using llvm::GVTOP;
using llvm::PTOGV;

typedef llvm::GenericValue GV;
typedef std::vector<GV> ArgArray;
#define CHECK_FD(fd) do { \
  GV result; \
  if (fd < 0 || fd > IMPL_MAX_FDS) { \
    result.IntVal = llvm::APInt(32, -EBADF); \
    return result; \
  } else if (fds[fd].extfd == FD_UNUSED) { \
    result.IntVal = llvm::APInt(32, -EBADF); \
    return result; \
  } \
} while(0)

inline int handle_errno(int result) {
  if (result >= 0)
    return result;
  else
    return -errno;
}


SeashellInterpreter_Impl::SuspendExn::SuspendExn(const SeashellInterpreter_Impl* impl) {
  if (impl->state != INTERP_CONTINUE) {
    val::module_property("_RT_internal_error")(val(std::string("Can't call a suspended function while not interpreting main body!")));
    throw std::runtime_error("Suspending function while not interpreting main body!");
  } else {
    val::module_property("_RT_suspend_function")(val(std::string(impl->resume.F->getName())));
  }
}

/** int32_t _seashell_RT_read(int32_t fd, void *buffer, uint32_t size). */
GV SeashellInterpreter_Impl::_RT_read(const ArgArray &Args) {
  GV result;
  result.IntVal = llvm::APInt(32, -EINVAL);
  int32_t fd = Args[0].IntVal.getSExtValue();
  void* buffer = GVTOP(Args[1]);
  uint32_t size = Args[2].IntVal.getZExtValue();
  CHECK_FD(fd);
  
  if (fds[fd].extfd == FD_INTERNAL && fds[fd].intfd == 0) {
    /** Read from stdin. */
    val syscall_result = val::module_property("_RT_stdin_read")(val(static_cast<uint32_t>(Args[2].IntVal.getZExtValue())));
    /** Two cases: string, 0, or -1 */
    if (syscall_result.strictlyEquals(val(0))) {
      /** EOF */
      result.IntVal = llvm::APInt(32, 0);
    } else if (syscall_result.strictlyEquals(val(-1))) {
      /** No data, resume later. */
      throw SuspendExn(this);
    } else {
      std::string read = syscall_result.as<std::string>();
      memcpy(GVTOP(Args[1]), read.c_str(), read.size());
      result.IntVal = llvm::APInt(32, read.size());
    }
  } else if(fds[fd].extfd != FD_INTERNAL) {
    /** TODO: Check if buffer is valid (eventually). */
    result.IntVal = llvm::APInt(32, handle_errno(read(fds[fd].extfd, buffer, size)));
  }
  return result;
}

/** _seashell_RT_write(int32_t fd, const void* buffer, uint33_t size). */
GV SeashellInterpreter_Impl::_RT_write(const ArgArray &Args) {
  GV result;
  result.IntVal = llvm::APInt(32, -EINVAL);
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
    result.IntVal = llvm::APInt(32, handle_errno(write(fds[fd].extfd, buffer, size)));
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
  throw SuspendExn(this);
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
    result.IntVal = llvm::APInt(32, -ENOMEM);
  }
  return result;
}

/** int32_t _seashell_RT_fstat(int32_t fd, int32_t *mode, uint64_t *size, int64_t *mtime, int64_t *atime, int64_t *ctime) */
GV SeashellInterpreter_Impl::_RT_stat(const ArgArray& Args) {
  GV result;
  int32_t fd = Args[0].IntVal.getSExtValue();
  int32_t* mode = static_cast<int32_t*>(GVTOP(Args[1]));
  uint64_t* size = static_cast<uint64_t*>(GVTOP(Args[2]));
  int64_t* mtime = static_cast<int64_t*>(GVTOP(Args[3]));
  int64_t* atime = static_cast<int64_t*>(GVTOP(Args[4]));
  int64_t* ctime = static_cast<int64_t*>(GVTOP(Args[5]));
  CHECK_FD(fd);

  if (fds[fd].extfd == FD_INTERNAL && fds[fd].intfd >= 0 && fds[fd].intfd <= 2) {
    *mode = S_IFCHR;
    if (fds[fd].intfd == 0)
      *mode |= S_IRUSR;
    else
      *mode |= S_IWUSR;
    *size = *mtime = *atime = *ctime = 0; 
    result.IntVal = llvm::APInt(32, 0);
  } else {
    struct stat buf;
    result.IntVal = llvm::APInt(32, handle_errno(fstat(fds[fd].extfd, &buf)));
    *mode = buf.st_mode;
    *size = buf.st_size;
    *mtime = buf.st_mtime;
    *atime = buf.st_atime;
    *ctime = buf.st_ctime;
  }
   
  return result; 
}

/** int32_t _seashell_RT_stat(const char* name, int32_t *mode, uint64_t *size, int64_t *mtime, int64_t *atime, int64_t *ctime) */
GV SeashellInterpreter_Impl::_RT_fstat(const ArgArray& Args) {
  GV result;
  const char* name = static_cast<const char*>(GVTOP(Args[1]));
  int32_t* mode = static_cast<int32_t*>(GVTOP(Args[1]));
  uint64_t* size = static_cast<uint64_t*>(GVTOP(Args[2]));
  int64_t* mtime = static_cast<int64_t*>(GVTOP(Args[3]));
  int64_t* atime = static_cast<int64_t*>(GVTOP(Args[4]));
  int64_t* ctime = static_cast<int64_t*>(GVTOP(Args[5]));

  if (strcmp(name, "/dev/stdout") == 0 || strcmp(name, "/dev/stderr") == 0) {
    *mode = S_IFCHR | S_IWUSR;
    *size = *mtime = *atime = *ctime = 0; 
    result.IntVal = llvm::APInt(32, 0);
  } else if (strcmp(name, "/dev/stdin") == 0) {
    *mode = S_IFCHR | S_IRUSR;
    *size = *mtime = *atime = *ctime = 0; 
    result.IntVal = llvm::APInt(32, 0);
  } else {
    struct stat buf;
    result.IntVal = llvm::APInt(32, handle_errno(stat(name, &buf)));
    *mode = buf.st_mode;
    *size = buf.st_size;
    *mtime = buf.st_mtime;
    *atime = buf.st_atime;
    *ctime = buf.st_ctime;
  }
   
  return result; 
}

/** int32_t _seashell_RT_gettimeofday(int64_t* seconds, int64_t* microseconds, int32_t *correction, int32_t *dst) */
GV SeashellInterpreter_Impl::_RT_gettimeofday(const ArgArray &ArgVals) {
  GV result;
  struct timeval tv;
  struct timezone tz;
  int64_t* seconds = static_cast<int64_t*>(GVTOP(ArgVals[0]));
  int64_t* mseconds = static_cast<int64_t*>(GVTOP(ArgVals[1]));
  int32_t* corr = static_cast<int32_t*>(GVTOP(ArgVals[2]));
  int32_t* dst = static_cast<int32_t*>(GVTOP(ArgVals[3]));

  result.IntVal = llvm::APInt(32, handle_errno(gettimeofday(&tv, &tz)));
  *seconds = tv.tv_sec;
  *mseconds = tv.tv_usec;
  *corr = tz.tz_minuteswest;
  *dst = tz.tz_dsttime;
  return result;
}

/** int32_t _seashell_RT_open(const char* name, int32_t flags, int32_t mode) */
GV SeashellInterpreter_Impl::_RT_open(const ArgArray &Args) {
  GV result;
  const char *name = static_cast<char*>(GVTOP(Args[0]));
  int32_t flags = Args[1].IntVal.getSExtValue();
  int32_t mode = Args[1].IntVal.getSExtValue();
  int32_t nextfd = 0;

  for (; nextfd < IMPL_MAX_FDS; nextfd++) {
    if (fds[nextfd].extfd == FD_UNUSED)
      break;
  }
  if (nextfd == IMPL_MAX_FDS) {
    nextfd = -EMFILE;
  }
  if (nextfd > 0) {
    if (strcmp(name, "/dev/stdout") == 0) {
      fds[nextfd].extfd = FD_INTERNAL;
      fds[nextfd].intfd = 1;
    } else if (strcmp(name, "/dev/stderr") == 0) {
      fds[nextfd].extfd = FD_INTERNAL;
      fds[nextfd].intfd = 2; 
    } else if (strcmp(name, "/dev/stdin") == 0) {
      fds[nextfd].extfd = FD_INTERNAL;
      fds[nextfd].intfd = 0; 
    } else {
      int32_t fd_or_error = handle_errno(open(name, flags, mode));
      if (fd_or_error < 0) {
        nextfd = fd_or_error;
      }
      fds[nextfd].extfd = fd_or_error;
    }
  }
  result.IntVal = llvm::APInt(32, nextfd);
  return result;
}

/** int _seashell_RT_close(int32_t fd) */
GV SeashellInterpreter_Impl::_RT_close(const ArgArray &Args) {
  GV result;
  int32_t fd = Args[0].IntVal.getSExtValue();
  CHECK_FD(fd);

  result.IntVal = llvm::APInt(32, 0);
  if (fds[fd].extfd > 0)
    result.IntVal = llvm::APInt(32, handle_errno(close(fds[fd].extfd)));
  fds[fd].extfd = FD_UNUSED;

  return result;
}

/** int _seashell_RT_lseek(int32_t fd, int64_t ptr, int32_t dir) */
GV SeashellInterpreter_Impl::_RT_lseek(const ArgArray &Args) {
  GV result;
  int32_t fd = Args[0].IntVal.getSExtValue();
  int64_t ptr = Args[0].IntVal.getSExtValue();
  int32_t dir = Args[0].IntVal.getSExtValue();
  CHECK_FD(fd);

  if (fds[fd].extfd == FD_INTERNAL) {
    result.IntVal = llvm::APInt(32, -ESPIPE);
  } else {
    result.IntVal = llvm::APInt(32, handle_errno(lseek(fds[fd].extfd, ptr, dir)));
  }
  return result;
}

/** int _seashell_RT_isatty(int32_t fd) */
GV SeashellInterpreter_Impl::_RT_isatty(const ArgArray &Args) {
  GV result;
  int32_t fd = Args[0].IntVal.getSExtValue();
  CHECK_FD(fd);
  if (fds[fd].extfd == FD_INTERNAL) {
    result.IntVal = llvm::APInt(32, 1);
  } else {
    result.IntVal = llvm::APInt(32, handle_errno(isatty(fds[fd].extfd)));
  }
  return result;
}

/** int32_t _seashell_RT_link(const char *old, const char *newl) */
GV SeashellInterpreter_Impl::_RT_link(const ArgArray &Args) {
  GV result;
  const char *old = static_cast<const char*>(GVTOP(Args[0]));
  const char *newl = static_cast<const char*>(GVTOP(Args[1]));
  result.IntVal = llvm::APInt(32, handle_errno(link(old, newl)));
  return result;
}

/** int32_t unlink(const char* old) */
GV SeashellInterpreter_Impl::_RT_unlink(const ArgArray &Args) {
  GV result;
  const char *old = static_cast<const char*>(GVTOP(Args[0]));
  result.IntVal = llvm::APInt(32, handle_errno(unlink(old)));
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
  if (ifunc != ExtFuncs.end()) {
    result = (this->*(ifunc->second))(resume.ArgVals);
  }
  /** unknown call. */
  else {
    result.IntVal = llvm::APInt(32, -EINVAL);
  }
  
  resume.F = nullptr;
  return result;
}

/** resume: int32_t _seashell_RT_suspend */
GV SeashellInterpreter_Impl::_RT_resume_suspend(const ArgArray& Args) {
  GV result;
  result.IntVal = llvm::APInt(32, val::module_property("_RT_extcall_result").as<int>());
  return result;
}

/** resume: int32_t _seashell_RT_read */
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
    result.IntVal = llvm::APInt(32, -EINVAL);
  }
  popStackAndReturnValueToCaller(resume.F->getReturnType(), result);
  resume.F = nullptr;
}
void SeashellInterpreter_Impl::exitCalled(int result) {
  if (state == INTERP_EXIT) {
    /** Ignore multiple exit() calls. */
    return;
  }
  // Set state to INTERP_EXIT
  state = INTERP_EXIT;
  // Clear the execution stack.
  ECStack.clear();
  runAtExitHandlers();
  // Run static destructors.
  runStaticConstructorsDestructors(true);
  // Set result
  result_ = result;
  // Toss exception to exit, reset emscripten stack.
  throw ExitExn();
}
void SeashellInterpreter_Impl::exitCalled(GV V) {
  exitCalled(V.IntVal.sextOrTrunc(32).getSExtValue());
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
