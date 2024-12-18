/*
Written by me
*/
#include <cassert>
#include <iostream>

#include "MITScript.h"
#include "antlr4-runtime.h"
#include "parser.h"


int main(int argc, const char *argv[]) {
  if (argc != 2) {
    std::cout << "Usage: mitscript-parser <filename>\n";
    return -1;
  }

  std::ifstream file;
  file.open(argv[1]);

  if (!file.is_open()) {
    std::cout << "Failed to open file: " << argv[1] << "\n";
    return -1;
  }

  // Create lexer
  antlr4::ANTLRInputStream input(file);
  MITScript lexer(&input);
  antlr4::CommonTokenStream tokens(&lexer);

  // Load all tokens within the file to a buffer
  tokens.fill();

  Parser parser;
#ifdef DEBUG
  std::cout << "------------------------------------" << std::endl;
  parser.printTokenStream(lexer , tokens);
#endif

  AST::Program * program = parser.ParseProgram(tokens);

  int result = (program == nullptr? 0:1);


  std::cout << "Parse status: " << result << "\n";

  return (result ? 0 : 1);
}
