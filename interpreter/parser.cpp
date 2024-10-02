/*
Written by me
*/
#include <cassert>
#include <iostream>
// #define PRETTY_PRINT_DEBUG
// #define DEBUG
// #define PRINT_DEBUG

#include "MITScript.h"
#include "antlr4-runtime.h"
#include "src/nonterminals.h"
#include "AST.h"
#include "PrettyPrinter.h"
#include "src/TreeConverter.cpp"

// Helper function for reporting errors

//Helper function for visualizing tokens



class Parser{
public:
    void printTokenStream(MITScript & lexer, antlr4::CommonTokenStream & tokens){
    // Print source as text
    std::cout << "source: " << tokens.getText() << "\n";

    // Print the tokens
    std::cout << "tokens: ";

    const auto &vocab = lexer.getVocabulary();
    for (size_t i = 0; i < tokens.size(); ++i) {
        if (i != 0) {
        std::cout << " ";
        }

        const auto token = tokens.get(i);
        std::cout << vocab.getSymbolicName(token->getType());
    }
    std::cout << "\n";

    }

    AST::Program * ParseProgram(antlr4::CommonTokenStream & tokens){
        AST::Program * result = nullptr;
        ProgramNode* cst_tree;
        try{
            cst_tree = Program(tokens);
#ifdef DEBUG
            std::cout << "------------------------------------" << std::endl;
            std::cout << "CST:    " << cst_tree->to_string() << "\n";
#endif

            CSTConverter converter = CSTConverter();
            AST::Program* program = converter.convert(*cst_tree);

            PrettyPrinter printer;

            std::cout << "AST:        " << std::endl;
            program->accept(printer);

            result = program;
        }
        catch(const std::runtime_error& err){
            std::cout << err.what();
        }
        return result;
    }

};



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

  // This is a cartoon of the output pattern
  // you should instead expect for your implementation
  // Parse the program, producing a Program AST node
  return (result ? 0 : 1);
}
