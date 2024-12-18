#include <iostream>

#include "bc-lexer.h"
#include "bc-parser.h"
#include "prettyprinter.h"

#include "parser.h"

#include "compiler.h"
#include "interpreter.h"


int main(int argc, const char *argv[]) {
  // We will now have the interpreter take the input
  // program from a file
  if (argc != 3) {
    std::cout << "Usage: mitscript -s/-b <filename>\n";
    return -1;
  }
  if (argv[1] != std::string("-s") && argv[1] != std::string("-b")) {
    std::cout << "Usage: mitscript -s/-b <filename>\n";
    return -1;
  }

  Function* bytecode_program;
  if (argv[1] == std::string("-s")) {
    std::ifstream file;
    file.open(argv[2]);

    if (!file.is_open()) {
        std::cout << "Failed to open file: " << argv[2] << "\n";
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
      return 1;
    }

    Compiler compiler;
    bytecode_program = compiler.compile(*program);

    if (bytecode_program == nullptr) {
      return 1;
    }
  } else {
    FILE* file = fopen(argv[2], "r");
    if (file == nullptr) {
        std::cout << "Failed to open file: " << argv[1] << "\n";
        return -1;
    }
    void* scanner;
    bclex_init(&scanner);
    bcset_in(file, scanner);

    int rvalue = bcparse(scanner, bytecode_program);
    if (rvalue == 1) {
      std::cout << "Parsing failed" << std::endl;
      return 1;
    }
  }

  Interpreter* interpreter = new Interpreter();
  try {
      interpreter->executeProgram(bytecode_program);
  }
  catch (InterpreterException& exception)
  {
      std::cout << exception.message() << "\n";
      return 1;
  }

  return 0;
}
