#ifndef SRC_LAMSCRIPT_LAMSCRIPT_H_
#define SRC_LAMSCRIPT_LAMSCRIPT_H_

#include <fstream>
#include <ios>
#include <iostream>
#include <vector>

#include <Lamscript/Interpreter.h>
#include <Lamscript/errors/RuntimeError.h>
#include <Lamscript/parsing/Token.h>

namespace lamscript {

class Lamscript {
 public:
  static void Run(const std::string& source);
  static void RunFile(const std::string& file_path);
  static void RunPrompt();
  static void Error(int line, const std::string& message);
  static void Error(parsing::Token token, const std::string& message);
  static void RuntimeError(lamscript::RuntimeError error);
  static void Report(
      int line, const std::string& where, const std::string& message);
 private:
  static Interpreter interpreter_;
  static bool had_error_, had_runtime_error_;
};


}  // namespace lamscript

#endif  // SRC_LAMSCRIPT_LAMSCRIPT_H_
