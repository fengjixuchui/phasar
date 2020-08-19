/******************************************************************************
 * Copyright (c) 2017 Philipp Schubert.
 * All rights reserved. This program and the accompanying materials are made
 * available under the terms of LICENSE.txt.
 *
 * Contributors:
 *     Philipp Schubert and others
 *****************************************************************************/

/*
 * Configuration.cpp
 *
 *  Created on: 04.05.2017
 *      Author: philipp
 */

#include <fstream>

#include "boost/algorithm/string/classification.hpp"
#include "boost/algorithm/string/split.hpp"
#include "boost/filesystem.hpp"

#include "phasar/Config/Configuration.h"
#include "phasar/Config/Version.h"

using namespace psr;

namespace psr {

PhasarConfig::PhasarConfig() {
  loadGlibcSpecialFunctionNames();
  loadLLVMSpecialFunctionNames();

  // Insert allocation operators
  special_function_names.insert({"_Znwm", "_Znam", "_ZdlPv", "_ZdaPv"});
}

std::string PhasarConfig::readConfigFile(const std::string &Path) {
  // We use a local file reading function to make phasar_config independent of
  // other phasar libraries.
  if (boost::filesystem::exists(Path) &&
      !boost::filesystem::is_directory(Path)) {
    std::ifstream Ifs(Path, std::ios::binary);
    if (Ifs.is_open()) {
      Ifs.seekg(0, std::ifstream::end);
      size_t FileSize = Ifs.tellg();
      Ifs.seekg(0, std::ifstream::beg);
      std::string Content(FileSize + 1, '\0');
      Ifs.read(const_cast<char *>(Content.data()), FileSize);
      return Content;
    }
  }
  throw std::ios_base::failure("could not read file: " + Path);
}

void PhasarConfig::loadGlibcSpecialFunctionNames() {
  const std::string GLIBCFunctionListFilePath =
      ConfigurationDirectory() + GLIBCFunctionListFileName;

  if (boost::filesystem::exists(GLIBCFunctionListFilePath)) {
    // Load glibc function names specified in the config file
    std::vector<std::string> GlibcFunctions;
    std::string Glibc = readConfigFile(GLIBCFunctionListFilePath);
    // Insert glibc function names
    boost::split(GlibcFunctions, Glibc, boost::is_any_of("\n"),
                 boost::token_compress_on);

    special_function_names.insert(GlibcFunctions.begin(), GlibcFunctions.end());
  } else {
    // Add default glibc function names
    special_function_names.insert({"_exit"});
  }
}

void PhasarConfig::loadLLVMSpecialFunctionNames() {
  const std::string LLVMFunctionListFilePath =
      ConfigurationDirectory() + LLVMIntrinsicFunctionListFileName;
  if (boost::filesystem::exists(LLVMFunctionListFilePath)) {
    // Load LLVM function names specified in the config file
    std::string LLVMIntrinsics = readConfigFile(LLVMFunctionListFilePath);

    std::vector<std::string> LLVMIntrinsicFunctions;
    boost::split(LLVMIntrinsicFunctions, LLVMIntrinsics, boost::is_any_of("\n"),
                 boost::token_compress_on);

    // Insert llvm intrinsic function names
    special_function_names.insert(LLVMIntrinsicFunctions.begin(),
                                  LLVMIntrinsicFunctions.end());
  } else {
    // Add default LLVM function names
    special_function_names.insert({"llvm.va_start"});
  }
}

const std::string PhasarConfig::PhasarDir = std::string([]() {
  std::string CurrPath = boost::filesystem::current_path().string();
  size_t I = CurrPath.rfind("build", CurrPath.length());
  return CurrPath.substr(0, I);
}());

PhasarConfig &PhasarConfig::getPhasarConfig() {
  static PhasarConfig PC;
  return PC;
}

} // namespace psr
