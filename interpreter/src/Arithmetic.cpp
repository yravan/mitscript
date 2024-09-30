#include "nonterminals.h"

ArithmeticPrimeNode * ArithmeticPrime(antlr4::CommonTokenStream &tokens){
  antlr4::Token *op_token = tokens.get(tokens.index());
  switch (op_token->getType()) {
    case MITScript::PLUS:
    case MITScript::MINUS: {
      tokens.consume();

      return new ArithmeticPrimeNode(op_token, Product(tokens), ArithmeticPrime(tokens));
    }
    default: {
      return NULL;
    }
  }
  assert(0);
}

ArithmeticNode * Arithmetic(antlr4::CommonTokenStream &tokens){
  return new ArithmeticNode(Product(tokens), ArithmeticPrime(tokens));
}

ProductPrimeNode * ProductPrime(antlr4::CommonTokenStream &tokens){
  antlr4::Token *op_token = tokens.get(tokens.index());
  switch (op_token->getType()) {
    case MITScript::MUL:
    case MITScript::DIV: {
      tokens.consume();

      return new ProductPrimeNode(op_token, Unit(tokens), ProductPrime(tokens));
    }
    default: {
      return NULL;
    }
  }
  assert(0);
}

ProductNode * Product(antlr4::CommonTokenStream &tokens){
  return new ProductNode(Unit(tokens), ProductPrime(tokens));
}
