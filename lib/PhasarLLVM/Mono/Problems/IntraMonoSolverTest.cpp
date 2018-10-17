/******************************************************************************
 * Copyright (c) 2017 Philipp Schubert.
 * All rights reserved. This program and the accompanying materials are made
 * available under the terms of LICENSE.txt.
 *
 * Contributors:
 *     Philipp Schubert and others
 *****************************************************************************/

/*
 * MonoSolverTest.cpp
 *
 *  Created on: 06.06.2017
 *      Author: philipp
 */

#include <algorithm>
#include <iostream>

#include <llvm/IR/Instruction.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Value.h>

#include <phasar/PhasarLLVM/ControlFlow/LLVMBasedCFG.h>
#include <phasar/Utils/LLVMShorthands.h>

#include <phasar/PhasarLLVM/Mono/Problems/IntraMonoSolverTest.h>
using namespace std;
using namespace psr;

namespace psr {

IntraMonoSolverTest::IntraMonoSolverTest(LLVMBasedCFG &Cfg,
                                         const llvm::Function *F)
    : IntraMonoProblem<const llvm::Instruction *, const llvm::Value *,
                       const llvm::Function *, LLVMBasedCFG &>(Cfg, F) {}

MonoSet<const llvm::Value *>
IntraMonoSolverTest::join(const MonoSet<const llvm::Value *> &Lhs,
                          const MonoSet<const llvm::Value *> &Rhs) {
  cout << "IntraMonoSolverTest::join()\n";
  MonoSet<const llvm::Value *> Result;
  set_union(Lhs.begin(), Lhs.end(), Rhs.begin(), Rhs.end(),
            inserter(Result, Result.begin()));
  return Result;
}

bool IntraMonoSolverTest::sqSubSetEqual(
    const MonoSet<const llvm::Value *> &Lhs,
    const MonoSet<const llvm::Value *> &Rhs) {
  cout << "IntraMonoSolverTest::sqSubSetEqual()\n";
  return includes(Rhs.begin(), Rhs.end(), Lhs.begin(), Lhs.end());
}

MonoSet<const llvm::Value *>
IntraMonoSolverTest::flow(const llvm::Instruction *S,
                          const MonoSet<const llvm::Value *> &In) {
  cout << "IntraMonoSolverTest::flow()\n";
  MonoSet<const llvm::Value *> Result;

  if (auto Alloc = llvm::dyn_cast<llvm::AllocaInst>(S)) {
    Result.insert(Alloc);
  }

  Result.insert(In.begin(), In.end());
  if (const auto Store = llvm::dyn_cast<llvm::StoreInst>(S)) {
    Result.insert(Store);
  }
  return Result;
}

MonoMap<const llvm::Instruction *, MonoSet<const llvm::Value *>>
IntraMonoSolverTest::initialSeeds() {
  cout << "IntraMonoSolverTest::initialSeeds()\n";
  return {};
}

string IntraMonoSolverTest::DtoString(const llvm::Value *d) {
  return llvmIRToString(d);
}

} // namespace psr
