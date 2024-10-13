/*
Written by me
*/
#include "nonterminals.h"
#include "CST.h"

ArithmeticPrimeNode* ArithmeticPrime(antlr4::CommonTokenStream &tokens) {
    DEBUG_PRINT("Entering ArithmeticPrime() with token " + tokens.get(tokens.index())->getText());
    ArithmeticPrimeNode* result = nullptr;
    antlr4::Token *op_token = tokens.get(tokens.index());
    switch (op_token->getType()) {
        case MITScript::PLUS:
        case MITScript::MINUS: {
            tokens.consume();
            auto product = Product(tokens);
            auto rest = ArithmeticPrime(tokens);
            result = new ArithmeticPrimeNode(op_token, product, rest);
            return result;
        }
    }
    DEBUG_PRINT("Exiting ArithmeticPrime() with operator: " + op_token->getText() + " with token " + tokens.get(tokens.index())->getText());
    return result;
}

ArithmeticNode* Arithmetic(antlr4::CommonTokenStream &tokens) {
    DEBUG_PRINT("Entering Arithmetic() with token " + tokens.get(tokens.index())->getText());
    auto prod = Product(tokens);
    auto rest = ArithmeticPrime(tokens);
    ArithmeticNode* result = new ArithmeticNode(prod, rest);
    DEBUG_PRINT("Exiting Arithmetic() with token " + tokens.get(tokens.index())->getText());
    return result;
}

ProductPrimeNode* ProductPrime(antlr4::CommonTokenStream &tokens) {
    DEBUG_PRINT("Entering ProductPrime() with token " + tokens.get(tokens.index())->getText());
    antlr4::Token *op_token = tokens.get(tokens.index());
    ProductPrimeNode* result = nullptr;
    switch (op_token->getType()) {
        case MITScript::MUL:
        case MITScript::DIV: 
            tokens.consume();
            auto unit = Unit(tokens);
            auto rest = ProductPrime(tokens);
            result = new ProductPrimeNode(op_token, unit, rest);
            return result;
    }
    DEBUG_PRINT("Exiting ProductPrime() with operator: " + op_token->getText() + " with token " + tokens.get(tokens.index())->getText());
    return result;
}

ProductNode* Product(antlr4::CommonTokenStream &tokens) {
    DEBUG_PRINT("Entering Product() with token " + tokens.get(tokens.index())->getText());
    auto unit = Unit(tokens);
    auto rest = ProductPrime(tokens);
    ProductNode* result = new ProductNode(unit, rest);
    DEBUG_PRINT("Exiting Product() with token " + tokens.get(tokens.index())->getText());
    return result;
}


ConstantNode* Constant(antlr4::CommonTokenStream &tokens) {
    DEBUG_PRINT("Entering Constant() with token " + tokens.get(tokens.index())->getText());
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
            DEBUG_PRINT("Exiting Constant() with token " + tokens.get(tokens.index())->getText());
            return result;
        
        default: {
            reportError(*token);
        }
    }
    return nullptr;
}
