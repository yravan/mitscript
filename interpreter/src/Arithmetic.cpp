/*
Written by me
*/
#include "nonterminals.h"
#include "CST.h"

ArithmeticPrimeNode* ArithmeticPrime(antlr4::CommonTokenStream &tokens) {
#ifdef DEBUG
    std::cout << "Entering ArithmeticPrime()" << " with token " << tokens.get(tokens.index())->getText() << std::endl ;
#endif
    antlr4::Token *op_token = tokens.get(tokens.index());
    switch (op_token->getType()) {
        case MITScript::PLUS:
        case MITScript::MINUS: {
            tokens.consume();
            ArithmeticPrimeNode* result = new ArithmeticPrimeNode(op_token, Product(tokens), ArithmeticPrime(tokens));
#ifdef DEBUG
            std::cout << "Exiting ArithmeticPrime() with operator: " << op_token->getText() << " with token " << tokens.get(tokens.index())->getText() << std::endl ;
#endif
            return result;
        }
        default: {
#ifdef DEBUG
            std::cout << "Exiting ArithmeticPrime() with nullptr" << " with token " << tokens.get(tokens.index())->getText() << std::endl ;
#endif
            return nullptr;
        }
    }
    assert(0);
}

ArithmeticNode* Arithmetic(antlr4::CommonTokenStream &tokens) {
#ifdef DEBUG
    std::cout << "Entering Arithmetic()" << " with token " << tokens.get(tokens.index())->getText() << std::endl ;
#endif
    ArithmeticNode* result = new ArithmeticNode(Product(tokens), ArithmeticPrime(tokens));
#ifdef DEBUG
    std::cout << "Exiting Arithmetic()" << " with token " << tokens.get(tokens.index())->getText() << std::endl ;
#endif
    return result;
}

ProductPrimeNode* ProductPrime(antlr4::CommonTokenStream &tokens) {
#ifdef DEBUG
    std::cout << "Entering ProductPrime()" << " with token " << tokens.get(tokens.index())->getText() << std::endl ;
#endif
    antlr4::Token *op_token = tokens.get(tokens.index());
    switch (op_token->getType()) {
        case MITScript::MUL:
        case MITScript::DIV: {
            tokens.consume();
            ProductPrimeNode* result = new ProductPrimeNode(op_token, Unit(tokens), ProductPrime(tokens));
#ifdef DEBUG
            std::cout << "Exiting ProductPrime() with operator: " << op_token->getText() << " with token " << tokens.get(tokens.index())->getText() << std::endl ;
#endif
            return result;
        }
        default: {
#ifdef DEBUG
            std::cout << "Exiting ProductPrime() with nullptr" << " with token " << tokens.get(tokens.index())->getText() << std::endl ;
#endif
            return nullptr;
        }
    }
    assert(0);
}

ProductNode* Product(antlr4::CommonTokenStream &tokens) {
#ifdef DEBUG
    std::cout << "Entering Product()" << " with token " << tokens.get(tokens.index())->getText() << std::endl ;
#endif
    ProductNode* result = new ProductNode(Unit(tokens), ProductPrime(tokens));
#ifdef DEBUG
    std::cout << "Exiting Product()" << " with token " << tokens.get(tokens.index())->getText() << std::endl ;
#endif
    return result;
}


ConstantNode* Constant(antlr4::CommonTokenStream &tokens) {
#ifdef DEBUG
    std::cout << "Entering Constant()" << " with token " << tokens.get(tokens.index())->getText() << std::endl ;
#endif
    antlr4::Token *token = tokens.get(tokens.index());
    ConstantNode* result;
    switch (token->getType()) {
        case MITScript::INT:
        case MITScript::STRING:
        case MITScript::TRUE:
        case MITScript::FALSE:
        case MITScript::NONE: 
            tokens.consume();
            result = new ConstantNode(token);
#ifdef DEBUG
            std::cout << "Exiting Constant() with token: " << token->getText() << " with token " << tokens.get(tokens.index())->getText() << std::endl ;
#endif
            return result;
        
        default: {
#ifdef DEBUG
            std::cout << "Exiting Constant() with error" << " with token " << tokens.get(tokens.index())->getText() << std::endl ;
#endif
            reportError(*token);
            return nullptr;
        }
    }

    assert(0); // This should ideally never be reached
}
