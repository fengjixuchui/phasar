/******************************************************************************
 * Copyright (c) 2017 Philipp Schubert.
 * All rights reserved. This program and the accompanying materials are made
 * available under the terms of LICENSE.txt.
 *
 * Contributors:
 *     Philipp Schubert and others
 *****************************************************************************/

/*
 * IFDSTaintAnalysis.h
 *
 *  Created on: 15.09.2016
 *      Author: pdschbrt
 */
#ifndef ANALYSIS_IFDS_IDE_PROBLEMS_IFDS_TAINT_ANALYSIS_IFDSTAINTANALYSIS_H_
#define ANALYSIS_IFDS_IDE_PROBLEMS_IFDS_TAINT_ANALYSIS_IFDSTAINTANALYSIS_H_

#include <algorithm>
#include <llvm/IR/CallSite.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Instruction.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>
#include <map>
#include <memory>
#include <phasar/PhasarLLVM/ControlFlow/LLVMBasedICFG.h>
#include <phasar/PhasarLLVM/IfdsIde/DefaultIFDSTabulationProblem.h>
#include <phasar/PhasarLLVM/IfdsIde/DefaultSeeds.h>
#include <phasar/PhasarLLVM/IfdsIde/FlowFunction.h>
#include <phasar/PhasarLLVM/IfdsIde/FlowFunctions/Gen.h>
#include <phasar/PhasarLLVM/IfdsIde/FlowFunctions/GenIf.h>
#include <phasar/PhasarLLVM/IfdsIde/FlowFunctions/GenAll.h>
#include <phasar/PhasarLLVM/IfdsIde/FlowFunctions/Identity.h>
#include <phasar/PhasarLLVM/IfdsIde/FlowFunctions/Kill.h>
#include <phasar/PhasarLLVM/IfdsIde/FlowFunctions/KillAll.h>
#include <phasar/PhasarLLVM/IfdsIde/LLVMFlowFunctions/MapFactsToCallee.h>
#include <phasar/PhasarLLVM/IfdsIde/LLVMFlowFunctions/MapFactsToCaller.h>
#include <phasar/PhasarLLVM/IfdsIde/SpecialSummaries.h>
#include <phasar/PhasarLLVM/IfdsIde/LLVMZeroValue.h>
#include <phasar/Utils/LLVMShorthands.h>
#include <phasar/Utils/Logger.h>
#include <set>
#include <string>
#include <utility>
#include <vector>
using namespace std;

// Functions that are considered as taint sensitve functions:
//
// Source functions - critical argument(s):
//  -fread - 0
//  -read - 1
//  -fgetc - ret
//  -fgets - 0, ret
//  -getc - ret
//  -getchar - ret
//  -ungetc - ret
//
// Sink functions:
//  -fwrite - 0
//  -write - 1
//  -printf - everything
//  -fputc - 0
//  -fputs - 0
//  -putc - 0
//  -putchar - 0
//  -puts - 0
class IFDSTaintAnalysis : public DefaultIFDSTabulationProblem<
                              const llvm::Instruction *, const llvm::Value *,
                              const llvm::Function *, LLVMBasedICFG &> {
private:
  vector<string> EntryPoints;

public:
  struct SourceFunction {
    string name;
    vector<unsigned> genargs;
    bool genreturn;
    SourceFunction(string n, vector<unsigned> gen, bool genret)
        : name(n), genargs(gen), genreturn(genret) {}
    SourceFunction(string n, bool genret) : name(n), genreturn(genret) {}
    friend ostream &operator<<(ostream &os, const SourceFunction &sf) {
      os << sf.name << ": ";
      for (auto arg : sf.genargs)
        os << arg << ",";
      return os << ", " << sf.genreturn << endl;
    }
  };
  struct SinkFunction {
    string name;
    vector<unsigned> sinkargs;
    SinkFunction(string n, vector<unsigned> sink) : name(n), sinkargs(sink) {}
    friend ostream &operator<<(ostream &os, const SinkFunction &sf) {
      os << sf.name << ": ";
      for (auto arg : sf.sinkargs)
        os << arg << ",";
      return os << endl;
    }
  };

  map<const llvm::Instruction *, set<const llvm::Value *>> Leaks;

  static const map<string, SourceFunction> Sources;

  static const map<string, SinkFunction> Sinks;

  IFDSTaintAnalysis(LLVMBasedICFG &icfg, vector<string> EntryPoints = {"main"});

  virtual ~IFDSTaintAnalysis() = default;

  shared_ptr<FlowFunction<const llvm::Value *>>
  getNormalFlowFunction(const llvm::Instruction *curr,
                        const llvm::Instruction *succ) override;

  shared_ptr<FlowFunction<const llvm::Value *>>
  getCallFlowFunction(const llvm::Instruction *callStmt,
                      const llvm::Function *destMthd) override;

  shared_ptr<FlowFunction<const llvm::Value *>>
  getRetFlowFunction(const llvm::Instruction *callSite,
                     const llvm::Function *calleeMthd,
                     const llvm::Instruction *exitStmt,
                     const llvm::Instruction *retSite) override;

  shared_ptr<FlowFunction<const llvm::Value *>>
  getCallToRetFlowFunction(const llvm::Instruction *callSite,
                           const llvm::Instruction *retSite) override;

  shared_ptr<FlowFunction<const llvm::Value *>>
  getSummaryFlowFunction(const llvm::Instruction *callStmt,
                         const llvm::Function *destMthd) override;

  map<const llvm::Instruction *, set<const llvm::Value *>>
  initialSeeds() override;

  const llvm::Value *createZeroValue() override;

  bool isZeroValue(const llvm::Value *d) const override;

  string DtoString(const llvm::Value *d) const override;

  string NtoString(const llvm::Instruction *n) const override;

  string MtoString(const llvm::Function *m) const override;
};

#endif /* ANALYSIS_IFDS_IDE_PROBLEMS_IFDS_TAINT_ANALYSIS_IFDSTAINTANALYSIS_HH_ \
        */
