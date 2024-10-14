#include <cassert>
#include <iostream>

#include "AST.h"
#include "MITScript.h"
#include "antlr4-runtime.h"
#include "src/TreeConverter.cpp"
#include "src/nonterminals.h"
#include "PrettyPrinter.h"

AST::Program *ParseProgram(antlr4::CommonTokenStream &tokens) {
  AST::Program * result = nullptr;
    ProgramNode* cst_tree;
    try{
        cst_tree = Program(tokens);
        DEBUG_PRINT("------------------------------------" + std::endl + "CST:    " + cst_tree->to_string() + std::endl);
        CSTConverter converter = CSTConverter();
        AST::Program* program = converter.convert(*cst_tree);
        result = program;
    }
    catch(const std::runtime_error& err){
        std::cout << err.what();
    }
    return result;
}

int main(int argc, const char *argv[]) {
  // We will now have the interpreter take the input
  // program from a file
  if (argc != 2) {
    std::cout << "Usage: mitscript <filename>\n";
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

  AST::Program *program = ParseProgram(tokens);

  if (program == nullptr) {
    // Print error messages if you'd like
    return 1;
  }

// Cartoon of calling your interpreter
#if 0

	try {
		Interpreter interp;
		program->accept(interp);
	}
	catch (InterpreterException& exception)
	{
    // Catch exception and show error message
		std::cout << exception.message() << "\n";
		return 1;
	}
#endif

  return 0;
}
