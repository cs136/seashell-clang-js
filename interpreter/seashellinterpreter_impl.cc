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
#include <llvm/IR/DiagnosticPrinter.h>
#include <llvm/Linker/Linker.h>
#include <emscripten.h>

SeashellInterpreter_Impl::SeashellInterpreter_Impl(std::unique_ptr<llvm::Module> M)
  : llvm::Interpreter(std::move(M)), result_(-1), heap_end(heap + MAX_HEAP_SIZE), state(INTERP_START) {
  /** Set up file descriptors. */
  fds[0].extfd = fds[1].extfd = fds[2].extfd = -1;
  fds[0].intfd = 0; fds[1].intfd = 1; fds[2].intfd = 2;
  for (int i = 3; i < IMPL_MAX_FDS; i++)
    fds[i].extfd = -2;
  /** Install system calls. */
  ExtFuncs["_seashell_RT_exit"] = &SeashellInterpreter_Impl::_RT_exit;
  ExtFuncs["_seashell_RT_close"] = &SeashellInterpreter_Impl::_RT_close;
  ExtFuncs["_seashell_RT_open"] = &SeashellInterpreter_Impl::_RT_open;
  ExtFuncs["_seashell_RT_read"] = &SeashellInterpreter_Impl::_RT_read;
  ExtFuncs["_seashell_RT_write"] = &SeashellInterpreter_Impl::_RT_write;
  ExtFuncs["_seashell_RT_isatty"] = &SeashellInterpreter_Impl::_RT_isatty;
  ExtFuncs["_seashell_RT_link"] = &SeashellInterpreter_Impl::_RT_link;
  ExtFuncs["_seashell_RT_lseek"] = &SeashellInterpreter_Impl::_RT_lseek;
  ExtFuncs["_seashell_RT_brk"] = &SeashellInterpreter_Impl::_RT_brk;
  ExtFuncs["_seashell_RT_brk_base"] = &SeashellInterpreter_Impl::_RT_brk_base;
  ExtFuncs["_seashell_RT_unlink"] = &SeashellInterpreter_Impl::_RT_unlink;
  ExtFuncs["_seashell_RT_stat"] = &SeashellInterpreter_Impl::_RT_stat;
  ExtFuncs["_seashell_RT_fstat"] = &SeashellInterpreter_Impl::_RT_fstat;
  ExtFuncs["_seashell_RT_gettimeofday"] = &SeashellInterpreter_Impl::_RT_gettimeofday;
  ExtFuncs["_seashell_RT_suspend"] = &SeashellInterpreter_Impl::_RT_suspend;
  ResumeFuncs["_seashell_RT_suspend"] = &SeashellInterpreter_Impl::_RT_resume_suspend;
  ResumeFuncs["_seashell_RT_read"] = &SeashellInterpreter_Impl::_RT_resume_read;
}

bool SeashellInterpreter_Impl::interpret() {
  try {
    if (resume.F) {
      resumeExternalFunction();
    }
    
    if (state == INTERP_START) {
      llvm::Function* start = FindFunctionNamed("_start");
      std::vector<std::string> argv = {"seashell-module", nullptr};
      runStaticConstructorsDestructors(false);
      // Run _start, and ignore its return value.
      state = INTERP_CONTINUE;
      runFunctionAsMain(start, argv, nullptr);
      exitCalled(-1);
    } else {
      // If this function returns, then we've successfully quit. 
      llvm::Interpreter::run();
      exitCalled(-1);
    }
  } catch(const SuspendExn& e) {
    return true; 
  } catch(const ExitExn& e) {
    return false;
  } catch(std::exception &e) {
    fprintf(stderr, "Got exception: %s\n", e.what());
    return false;
  }
  // Should never get here.
  return false;
}

bool SeashellInterpreter_Impl::add(std::unique_ptr<llvm::Module> N, std::string& Error) {
  llvm::Module* M = Modules.back().get();
  llvm::raw_string_ostream Stream(Error);
  llvm::DiagnosticPrinterRawOStream DP(Stream);
  bool Success = !llvm::Linker::LinkModules(M, N.get(), [&](const llvm::DiagnosticInfo &DI) { DI.print(DP); });
  Stream.flush();
  return Success;
}

