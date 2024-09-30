#include <cassert>
#include <iostream>

#include "MITScript.h"
#include "antlr4-runtime.h"

// Helper function for reporting errors
void reportError(antlr4::Token &token) {
  std::cout << "Unexpected Symbol (" << token.getLine() << ","
            << token.getCharPositionInLine() << "): " << token.getText()
            << "\n";
}

//Helper function for visualizing tokens
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

bool Constant(antlr4::CommonTokenStream & );// fully implemented
bool Unit(antlr4::CommonTokenStream&  ); // NOT fully implemented
bool Product(antlr4::CommonTokenStream & );// fully implemented
bool ProductPrime(antlr4::CommonTokenStream & );// fully implemented
bool Arithmetic(antlr4::CommonTokenStream & ); // fully implemented
bool ArithmeticPrime(antlr4::CommonTokenStream & ); // fully implemented
bool Statement(antlr4::CommonTokenStream & );
bool Program(antlr4::CommonTokenStream & );


bool Program(antlr4::CommonTokenStream &tokens){
  bool result = Arithmetic(tokens);
  antlr4::Token * last_token = tokens.get(tokens.index());
  if (last_token->getType() != MITScript::EOF){
    return false;
  }
  return result;
}

bool Arithmetic(antlr4::CommonTokenStream &tokens){
  return Product(tokens) && ArithmeticPrime(tokens);
}
bool ArithmeticPrime(antlr4::CommonTokenStream &tokens){
  antlr4::Token *op_token = tokens.get(tokens.index());
  switch (op_token->getType()) {
    case MITScript::PLUS:
    case MITScript::MINUS: {
      tokens.consume();

      return Product(tokens) && ArithmeticPrime(tokens);
    }
    default: {
      return true;
    }
  }
  assert(0);
  return false;
}

bool Product(antlr4::CommonTokenStream &tokens){
  return Unit(tokens) && ProductPrime(tokens);
}
bool ProductPrime(antlr4::CommonTokenStream &tokens){
  antlr4::Token *op_token = tokens.get(tokens.index());
  switch (op_token->getType()) {
    case MITScript::MUL:
    case MITScript::DIV: {
      tokens.consume();

      return Unit(tokens) && ProductPrime(tokens);
    }
    default: {
      return true;
    }
  }
  assert(0);
  return false;
}


bool Unit(antlr4::CommonTokenStream &tokens){
  // Get token at current index in buffer
  antlr4::Token *token = tokens.get(tokens.index());

  if (token->getType() == MITScript::MINUS) {
    tokens.consume();
  }

  return Constant(tokens);
}

bool Constant(antlr4::CommonTokenStream &tokens){
  // Get token at current index in buffer
  antlr4::Token *token = tokens.get(tokens.index());

  switch (token->getType()) {
    // check if token is integer constant
    case MITScript::INT:
    // check if token is string constant
    case MITScript::STRING:
    // check if token is boolean constant
    case MITScript::TRUE:
    case MITScript::FALSE:
    // check if token is None constant
    case MITScript::NONE:
      tokens.consume();
      return true;

    default: {
      reportError(*token);
      return false;
    }
  }

  // We shouldn't reach here
  assert(0);

  return false;
}


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

  printTokenStream(lexer , tokens);

  bool result = Program(tokens);

  std::cout << "Parse status: " << result << "\n";

  return (result ? 0 : 1);

  // This is a cartoon of the output pattern
  // you should instead expect for your implementation
#if 0
  // Parse the program, producing a Program AST node
  AST::Program* output = Program(tokens);

  // Create pretty print
  PrettyPrinter printer;

  // Print program
  program->accept(printer);

  // return appropriate return code
#endif
}
