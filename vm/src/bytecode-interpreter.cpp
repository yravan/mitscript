#include <iostream>

#include "bc-lexer.h"
#include "bc-parser.h"
#include "../bc-parser/prettyprinter.h"

#include "interpreter.h"

int main(int argc, const char *argv[]) {
  // We will now have the interpreter take the input
  // program from a file
  if (argc != 3) {
    std::cout << "Usage: mitscriptbc -b <filename>\n";
    return -1;
  }

  FILE* file = fopen(argv[2], "r");
  if (file == nullptr) {
      std::cout << "Failed to open file: " << argv[1] << "\n";
      return -1;
  }

  void* scanner;
  bclex_init(&scanner);
  bcset_in(file, scanner);

  Function* output;
  int rvalue = bcparse(scanner, output);
  if (rvalue == 1) {
    std::cout << "Parsing failed" << std::endl;
    return 1;
  }
  
  Interpreter* interpreter = new Interpreter();
  try {
      interpreter->executeProgram(output);
  }
  catch (InterpreterException& exception)
  {
      std::cout << exception.message() << "\n";
      return 1;
  }

  return 0;
}
