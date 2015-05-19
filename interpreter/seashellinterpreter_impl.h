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
#define FD_INTERNAL -1
#define FD_UNUSED -2

class SeashellInterpreter_Impl;

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
    int extfd; // FD_INTERNAL for internal (0, 1, 2) fd, FD_UNUSED for not used;
    int intfd; // Internal FD descriptor (0 = stdin, 1 = stdout, 2 = stderr)
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
  void exitCalled(llvm::GenericValue GV) override;
  void exitCalled(int result);
  void exitCalled();
protected:
  virtual void StoreValueToMemory(const llvm::GenericValue& Val, llvm::GenericValue* Ptr, llvm::Type* Ty) override;
  void resumeExternalFunction();
protected:
  /** Helper functions for implementing Runtime Library calls. */
  typedef llvm::GenericValue (SeashellInterpreter_Impl::*ExtFunc)(const std::vector<llvm::GenericValue> &Args);
  llvm::GenericValue _RT_exit(const std::vector<llvm::GenericValue> &ArgVals);
  llvm::GenericValue _RT_close(const std::vector<llvm::GenericValue> &ArgVals);
  llvm::GenericValue _RT_open(const std::vector<llvm::GenericValue> &ArgVals);
  llvm::GenericValue _RT_read(const std::vector<llvm::GenericValue> &ArgVals);
  llvm::GenericValue _RT_write(const std::vector<llvm::GenericValue> &ArgVals);
  llvm::GenericValue _RT_isatty(const std::vector<llvm::GenericValue> &ArgVals);
  llvm::GenericValue _RT_link(const std::vector<llvm::GenericValue> &ArgVals);
  llvm::GenericValue _RT_lseek(const std::vector<llvm::GenericValue> &ArgVals);
  llvm::GenericValue _RT_brk(const std::vector<llvm::GenericValue> &ArgVals);
  llvm::GenericValue _RT_brk_base(const std::vector<llvm::GenericValue> &ArgVals);
  llvm::GenericValue _RT_unlink(const std::vector<llvm::GenericValue> &ArgVals);
  llvm::GenericValue _RT_stat(const std::vector<llvm::GenericValue> &ArgVals);
  llvm::GenericValue _RT_fstat(const std::vector<llvm::GenericValue> &ArgVals);
  llvm::GenericValue _RT_gettimeofday(const std::vector<llvm::GenericValue> &ArgVals);
  llvm::GenericValue _RT_suspend(const std::vector<llvm::GenericValue> &ArgVals);
  llvm::GenericValue _RT_resume_suspend(const std::vector<llvm::GenericValue> &ArgVals);
  llvm::GenericValue _RT_resume_read(const std::vector<llvm::GenericValue> &ArgVals);
  std::map<std::string, ExtFunc> ExtFuncs; 
  std::map<std::string, ExtFunc> ResumeFuncs; 
};

#endif
