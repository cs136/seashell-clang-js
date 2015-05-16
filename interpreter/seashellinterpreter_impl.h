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

#ifndef __SEASHELL_INTERPRETER_IMPL__
#define __SEASHELL_INTERPRETER_IMPL__

#include <llvm/ExecutionEngine/Interpreter.h>
#include <llvm/IR/Module.h>

#define IMPL_MAX_FDS 1024   // 1024 fds
#define MAX_HEAP_SIZE 16384 // 16KB

class SeashellInterpreter_Impl : public llvm::Interpreter {
private:
  /** Function call to resume when calling a blocking
   *  (read/write) function. */
  struct FunctionCall {
    llvm::Function* F;
    std::vector<llvm::GenericValue> ArgVals;
    FunctionCall() : F(nullptr) {}
  } resume;
  
  /** Data Structures for maintaing state (file descriptors). */
  struct Fds {
    int extfd; // -1 for internal (0, 1, 2) fd, -2 for not used;
    int intfd; // Internal FD descriptor (0 = stdin, 1 = stdout, 2 = stderr)
    std::string buffer; // Buffer information for reads.
  } fds[IMPL_MAX_FDS];

  char heap[MAX_HEAP_SIZE];
  const char *heap_end;

  int result_;
public:
  bool add(std::unique_ptr<llvm::Module> M, std::string& error);
public:
  explicit SeashellInterpreter_Impl(std::unique_ptr<llvm::Module> M);
  void start();
  int result() const;

  virtual void run() override;
  virtual llvm::GenericValue callExternalFunction(llvm::Function *F,
                                                  const std::vector<llvm::GenericValue> &ArgVals) override;

  /** It'd be nice to do some basic checking/stack traces with these functions. */
  virtual void LoadValueFromMemory(llvm::GenericValue& Result, llvm::GenericValue* Ptr, llvm::Type* Ty) override;

  /** Run Exit */
  virtual void exitCalled(llvm::GenericValue GV) override;
  virtual void exitCalled(int result);
  virtual void exitCalled();
protected:
  virtual void StoreValueToMemory(const llvm::GenericValue& Val, llvm::GenericValue* Ptr, llvm::Type* Ty) override;
  virtual void resumeExternalFunction();
};

#endif
