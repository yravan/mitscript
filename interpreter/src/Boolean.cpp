/*
Written by me
*/
#include "nonterminals.h"
#include "CST.h"

BooleanPrimeNode* BooleanPrime(antlr4::CommonTokenStream &tokens) {
#ifdef DEBUG
    std::cout << "Entering BooleanPrime()" << " with token " << tokens.get(tokens.index())->getText() << std::endl ;
#endif
    antlr4::Token *token = tokens.get(tokens.index());
    if (token->getType() == MITScript::OR) {
        tokens.consume();
        auto conjunction = Conjunction(tokens);
        auto rest = BooleanPrime(tokens);
        BooleanPrimeNode* result = new BooleanPrimeNode(token,conjunction,rest);
#ifdef DEBUG
        std::cout << "Exiting BooleanPrime()" << " with token " << tokens.get(tokens.index())->getText() << std::endl ;
#endif
        return result;
    }
#ifdef DEBUG
    std::cout << "Exiting BooleanPrime() with nullptr" << " with token " << tokens.get(tokens.index())->getText() << std::endl ;
#endif
    return nullptr;
}

BooleanNode* Boolean(antlr4::CommonTokenStream &tokens) {
#ifdef DEBUG
    std::cout << "Entering Boolean()" << " with token " << tokens.get(tokens.index())->getText() << std::endl ;
#endif
    auto conjunction = Conjunction(tokens);
    auto rest = BooleanPrime(tokens);
    BooleanNode* result = new BooleanNode(conjunction,rest);
#ifdef DEBUG
    std::cout << "Exiting Boolean()" << " with token " << tokens.get(tokens.index())->getText() << std::endl ;
#endif
    return result;
}

ConjunctionPrimeNode* ConjunctionPrime(antlr4::CommonTokenStream &tokens) {
#ifdef DEBUG
    std::cout << "Entering ConjunctionPrime()" << " with token " << tokens.get(tokens.index())->getText() << std::endl ;
#endif
    antlr4::Token *token = tokens.get(tokens.index());
    if (token->getType() == MITScript::AND) {
        tokens.consume();
        auto bool_unit = BoolUnit(tokens);
        auto rest = ConjunctionPrime(tokens);
        ConjunctionPrimeNode* result = new ConjunctionPrimeNode(token, bool_unit, rest);
#ifdef DEBUG
        std::cout << "Exiting ConjunctionPrime()" << " with token " << tokens.get(tokens.index())->getText() << std::endl ;
#endif
        return result;
    }
#ifdef DEBUG
    std::cout << "Exiting ConjunctionPrime() with nullptr" << " with token " << tokens.get(tokens.index())->getText() << std::endl ;
#endif
    return nullptr;
}

ConjunctionNode* Conjunction(antlr4::CommonTokenStream &tokens) {
#ifdef DEBUG
    std::cout << "Entering Conjunction()" << " with token " << tokens.get(tokens.index())->getText() << std::endl ;
#endif
    auto bool_unit = BoolUnit(tokens);
    auto rest = ConjunctionPrime(tokens);
    ConjunctionNode* result = new ConjunctionNode(bool_unit, rest);
#ifdef DEBUG
    std::cout << "Exiting Conjunction()" << " with token " << tokens.get(tokens.index())->getText() << std::endl ;
#endif
    return result;
}

BoolUnitNode* BoolUnit(antlr4::CommonTokenStream &tokens) {
#ifdef DEBUG
    std::cout << "Entering BoolUnit()" << " with token " << tokens.get(tokens.index())->getText() << std::endl ;
#endif
    antlr4::Token *token = tokens.get(tokens.index());
    if (token->getType() == MITScript::NOT) {
        tokens.consume();
        BoolUnitNode* result = new BoolUnitNode(true, Predicate(tokens));
#ifdef DEBUG
        std::cout << "Exiting BoolUnit() with NOT" << " with token " << tokens.get(tokens.index())->getText() << std::endl ;
#endif
        return result;
    }
    BoolUnitNode* result = new BoolUnitNode(false, Predicate(tokens));
#ifdef DEBUG
    std::cout << "Exiting BoolUnit() with no NOT" << " with token " << tokens.get(tokens.index())->getText() << std::endl ;
#endif
    return result;
}

PredicateNode* Predicate(antlr4::CommonTokenStream &tokens) {
#ifdef DEBUG
    std::cout << "Entering Predicate()" << " with token " << tokens.get(tokens.index())->getText() << std::endl ;
#endif
    ArithmeticNode* arith_1 = Arithmetic(tokens);
    antlr4::Token *cop_token = tokens.get(tokens.index());
    switch (cop_token->getType()) {
        case MITScript::LT:
        case MITScript::GT:
        case MITScript::LE:
        case MITScript::GE:
        case MITScript::EQ: {
            tokens.consume();
            PredicateNode* result = new PredicateNode(arith_1, cop_token, Arithmetic(tokens));
#ifdef DEBUG
            std::cout << "Exiting Predicate() with comparison operator" << " with token " << tokens.get(tokens.index())->getText() << std::endl ;
#endif
            return result;
        }
        default: {
            PredicateNode* result = new PredicateNode(arith_1, nullptr, nullptr);
#ifdef DEBUG
            std::cout << "Exiting Predicate() with no comparison operator" << " with token " << tokens.get(tokens.index())->getText() << std::endl ;
#endif
            return result;
        }
    }
    assert(0);  // This should ideally never be reached
}