/*
Written by me
*/
#include "nonterminals.h"
#include "CST.h"

namespace CST{

    BooleanPrimeNode* BooleanPrime(antlr4::CommonTokenStream &tokens) {
        antlr4::Token *token = tokens.get(tokens.index());
        BooleanPrimeNode* result = nullptr;
        if (token->getType() == MITScript::OR) {
            tokens.consume();
            auto conjunction = Conjunction(tokens);
            auto rest = BooleanPrime(tokens);
            result = new BooleanPrimeNode(token, conjunction, rest);
        }
        if (result == nullptr){
        }else{
        }
        return result;
    }

    BooleanNode* Boolean(antlr4::CommonTokenStream &tokens) {
        auto conjunction = Conjunction(tokens);
        auto rest = BooleanPrime(tokens);
        BooleanNode* result = new BooleanNode(conjunction, rest);
        return result;
    }

    ConjunctionPrimeNode* ConjunctionPrime(antlr4::CommonTokenStream &tokens) {
        antlr4::Token *token = tokens.get(tokens.index());
        ConjunctionPrimeNode* result = nullptr;
        if (token->getType() == MITScript::AND) {
            tokens.consume();
            auto bool_unit = BoolUnit(tokens);
            auto rest = ConjunctionPrime(tokens);
            result = new ConjunctionPrimeNode(token, bool_unit, rest);
        }
        if (result == nullptr){
        }else{
        }
        return result;
    }

    ConjunctionNode* Conjunction(antlr4::CommonTokenStream &tokens) {
        auto bool_unit = BoolUnit(tokens);
        auto rest = ConjunctionPrime(tokens);
        ConjunctionNode* result = new ConjunctionNode(bool_unit, rest);
        return result;
    }

    BoolUnitNode* BoolUnit(antlr4::CommonTokenStream &tokens) {
        antlr4::Token *token = tokens.get(tokens.index());
        BoolUnitNode* result = nullptr;
        if (token->getType() == MITScript::NOT) {
            tokens.consume();
            result = new BoolUnitNode(true, Predicate(tokens));
        }
        else{
            result = new BoolUnitNode(false, Predicate(tokens));
        }
        return result;
    }

    PredicateNode* Predicate(antlr4::CommonTokenStream &tokens) {
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
                return result;
            }
            default: {
                PredicateNode* result = new PredicateNode(arith_1, nullptr, nullptr);
                return result;
            }
        }
        assert(0);  // This should ideally never be reached
    }
}
