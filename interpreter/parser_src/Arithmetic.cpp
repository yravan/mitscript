/*
Written by me
*/
#include "nonterminals.h"
#include "CST.h"

namespace CST{

    ArithmeticPrimeNode* ArithmeticPrime(antlr4::CommonTokenStream &tokens) {
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
        return result;
    }

    ArithmeticNode* Arithmetic(antlr4::CommonTokenStream &tokens) {
        auto prod = Product(tokens);
        auto rest = ArithmeticPrime(tokens);
        ArithmeticNode* result = new ArithmeticNode(prod, rest);
        return result;
    }

    ProductPrimeNode* ProductPrime(antlr4::CommonTokenStream &tokens) {
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
        return result;
    }

    ProductNode* Product(antlr4::CommonTokenStream &tokens) {
        auto unit = Unit(tokens);
        auto rest = ProductPrime(tokens);
        ProductNode* result = new ProductNode(unit, rest);
        return result;
    }


    ConstantNode* Constant(antlr4::CommonTokenStream &tokens) {
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
                return result;
            
            default: {
                reportError(*token);
            }
        }
        return nullptr;
    }
}
