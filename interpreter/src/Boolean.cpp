#include "nonterminals.h"

BooleanPrimeNode* BooleanPrime(antlr4::CommonTokenStream &tokens){
  antlr4::Token *token = tokens.get(tokens.index());
  if (token->getType() == MITScript::OR){
    tokens.consume();
    return new BooleanPrimeNode(token, Conjunction(tokens), BooleanPrime(tokens));
  }
  return NULL;
}

BooleanNode * Boolean(antlr4::CommonTokenStream &tokens){
  return new BooleanNode(Conjunction(tokens), BooleanPrime(tokens));
}


ConjunctionPrimeNode* ConjunctionPrime(antlr4::CommonTokenStream &tokens){
  antlr4::Token *token = tokens.get(tokens.index());
  if (token->getType() == MITScript::AND){
    tokens.consume();
    return new ConjunctionPrimeNode(token, BoolUnit(tokens), ConjunctionPrime(tokens));
  }
  return NULL;
}
ConjunctionNode * Conjunction(antlr4::CommonTokenStream &tokens){
  return new ConjunctionNode(BoolUnit(tokens), ConjunctionPrime(tokens));
}

BoolUnitNode * BoolUnit(antlr4::CommonTokenStream &tokens){
  antlr4::Token *token = tokens.get(tokens.index());
  if (token->getType() == MITScript::NOT){
    tokens.consume();
    return new BoolUnitNode(true, Predicate(tokens));
  }
  return new BoolUnitNode(false, Predicate(tokens));
  assert(0);
}

PredicateNode * Predicate(antlr4::CommonTokenStream &tokens){
  ArithmeticNode* arith_1 = Arithmetic(tokens);
  antlr4::Token *cop_token = tokens.get(tokens.index());
  switch (cop_token->getType()) {
    case MITScript::LT:
    case MITScript::GT:
    case MITScript::LE:
    case MITScript::GE:
    case MITScript::EQ:{
      tokens.consume();

      return new PredicateNode(arith_1, cop_token, Arithmetic(tokens));
    }
    default: {
      return new PredicateNode(arith_1, NULL, NULL);
    }
  }
  assert(0);
}

