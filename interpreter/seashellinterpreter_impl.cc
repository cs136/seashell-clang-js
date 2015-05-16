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
  : llvm::Interpreter(std::move(M)), result_(-1), heap_end(heap + MAX_HEAP_SIZE) {
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

void SeashellInterpreter_Impl::start() {
  llvm::Function* start = FindFunctionNamed("_start");
  std::vector<std::string> argv = {"seashell-module"};

  if (!start) {
    return;
  }

  runStaticConstructorsDestructors(false);
  // Run _start, and ignore its return value.
  runFunctionAsMain(start, argv, nullptr);
  exitCalled(-1);
}

