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

SeashellInterpreter_Impl::SeashellInterpreter_Impl(std::unique_ptr<llvm::Module> M)
  : llvm::Interpreter(std::move(M)) {
    fds[0].extfd = fds[1].extfd = fds[2].extfd = -1;
    for (int i = 3; i < IMPL_MAX_FDS; i++)
      fds[i].extfd = -2;
  }

void SeashellInterpreter_Impl::run() {
  llvm::Interpreter::run();
}

llvm::GenericValue SeashellInterpreter_Impl::callExternalFunction(llvm::Function* F,
                                                                  const std::vector<llvm::GenericValue> &ArgVals) {
  return llvm::Interpreter::callExternalFunction(F, ArgVals);
}

void SeashellInterpreter_Impl::LoadValueFromMemory(llvm::GenericValue& Result, llvm::GenericValue* Ptr, llvm::Type* Ty) {
  llvm::Interpreter::LoadValueFromMemory(Result, Ptr, Ty);
}

void SeashellInterpreter_Impl::StoreValueToMemory(const llvm::GenericValue& Val, llvm::GenericValue* Ptr, llvm::Type* Ty) {
  llvm::Interpreter::StoreValueToMemory(Val, Ptr, Ty);
}
