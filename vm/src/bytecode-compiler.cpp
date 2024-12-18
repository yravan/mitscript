#include <iostream>
#include <fstream>

#include "parser.h"

#include "compiler.h"
#include "prettyprinter.h"

int main(int argc, const char *argv[]) {
  // We will now have the interpreter take the input
  // program from a file
  if (argc != 2) {
    std::cout << "Usage: mitscriptc <filename>\n";
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
  AST::Program *program = parser.ParseProgram(tokens);

  if (program == nullptr) {
    // Print error messages if you'd like
    return 1;
  }

  Compiler compiler;
  Function* bytecode_program = compiler.compile(*program);

  if (bytecode_program == nullptr) {
    // Print error messages if you'd like
    return 1;
  }

  PrettyPrinter bytecodePrinter;
  bytecodePrinter.print(*bytecode_program, std::cout);

}
