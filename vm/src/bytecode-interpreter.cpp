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
  if (argc != 5) {
    std::cout << "Usage: mitscript -mem N -s/-b <filename>\n";
    return -1;
  }
  if (argv[1] != std::string("-mem")) {
    std::cout << "Usage: mitscript -mem N -s/-b <filename>\n";
    return -1;
  }
  if (argv[3] != std::string("-s") && argv[3] != std::string("-b")) {
    std::cout << "Usage: mitscript -mem N -s/-b <filename>\n";
    return -1;
  }

  int max_memory_mb = std::stoi(argv[2]) - 1;
  CollectedHeap heap(max_memory_mb * MEGABYTE_TO_BYTE);

  #ifdef DEBUG
  DEBUG_PRINT("Initial heap dump:");
  heap.dump();
  #endif

  Function* bytecode_program;
  if (argv[3] == std::string("-s")) {
    std::ifstream file;
    file.open(argv[4]);

    if (!file.is_open()) {
        std::cout << "Failed to open file: " << argv[4] << "\n";
        return -1;
    }

    #ifdef DEBUG
    DEBUG_PRINT("After opening file:");
    heap.dump();
    #endif
    // Create lexer
    antlr4::ANTLRInputStream input(file);
    MITScript lexer(&input);
    antlr4::CommonTokenStream tokens(&lexer);

    // Load all tokens within the file to a buffer
    tokens.fill();

    #ifdef DEBUG
    DEBUG_PRINT("After filling tokens:");
    heap.dump();
    #endif

    Parser parser;
    AST::Program *program = parser.ParseProgram(tokens);
    if (program == nullptr) {
      return 1;
    }

    #ifdef DEBUG
    DEBUG_PRINT("After parsing program:");
    heap.dump();
    #endif

    Compiler compiler(&heap);
    bytecode_program = compiler.compile(*program);

    #ifdef DEBUG
    DEBUG_PRINT("After compiling program:");
    heap.dump();
    #endif

    if (bytecode_program == nullptr) {
      return 1;
    }
    #ifdef DEBUG
    PrettyPrinter bytecodePrinter;
    bytecodePrinter.print(*bytecode_program, std::cout);
    #endif

  } else {
    FILE* file = fopen(argv[4], "r");
    if (file == nullptr) {
        std::cout << "Failed to open file: " << argv[4] << "\n";
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

  Interpreter interpreter(&heap);
  try {
      interpreter.executeProgram(bytecode_program);
  }
  catch (InterpreterException& exception)
  {
      std::cout << exception.message() << "\n";
      return 1;
  }

  return 0;
}
