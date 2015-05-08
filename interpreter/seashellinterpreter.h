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
#ifndef __SEASHELL_INTERPRETER__
#define __SEASHELL_INTERPRETER__

#include <string>

class SeashellInterpreter_Impl;
namespace llvm {class LLVMContext;}

class SeashellInterpreter {
  SeashellInterpreter_Impl* impl;
  llvm::LLVMContext* ctx;
  std::string source;
  std::string assemble_error_;
public:
  SeashellInterpreter(const std::string& source);
  ~SeashellInterpreter();
public:
  bool assemble();
  std::string assemble_error() const;
  void start();
  void continue_();
  int result() const;
};

#endif
