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
#include "seashellinterpreter.h"
#include "seashellinterpreter_impl.h"

#include <llvm/AsmParser/Parser.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/raw_os_ostream.h>

#include <time.h>
SeashellInterpreter::SeashellInterpreter() :
  impl(nullptr) {
  ctx = new llvm::LLVMContext();
  impl = new SeashellInterpreter_Impl(std::unique_ptr<llvm::Module>(new llvm::Module("seashell-module", *ctx)));
}

SeashellInterpreter::~SeashellInterpreter() {
  delete impl;
  delete ctx;
}

bool SeashellInterpreter::assemble(const std::string& source) {
  llvm::SMDiagnostic Err;
  llvm::raw_string_ostream os(assemble_error_);
  
  std::unique_ptr<llvm::Module> M = parseAssemblyString(source, Err, *ctx);

  if(!M.get()) {
    Err.print("seashell-interpreter", os);
    os.flush();
    return false;
  }

  if(llvm::verifyModule(*M.get(), &os)) {
    os.flush();
    return false;
  }

  return impl->add(std::move(M), assemble_error_);
}

void SeashellInterpreter::start() {
  impl->start();
}

void SeashellInterpreter::continue_() {
  impl->run();
}

int SeashellInterpreter::result() const {
  return impl->result();
}

std::string SeashellInterpreter::assemble_error() const {
  return assemble_error_;
}

#include <emscripten/bind.h>
using namespace emscripten;
EMSCRIPTEN_BINDINGS(seashell_interpreter) {
  class_<SeashellInterpreter>("SeashellInterpreter")
    .constructor<>()
    .function("assemble", &SeashellInterpreter::assemble)
    .function("assemble_error", &SeashellInterpreter::assemble_error)
    .function("start", &SeashellInterpreter::start)
    .function("continue_", &SeashellInterpreter::continue_)
    .function("result", &SeashellInterpreter::result);
}
