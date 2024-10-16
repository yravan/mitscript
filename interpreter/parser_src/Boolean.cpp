/*
Written by me
*/
#include "nonterminals.h"
#include "CST.h"

namespace CST{

    BooleanPrimeNode* BooleanPrime(antlr4::CommonTokenStream &tokens) {
        DEBUG_PRINT("Entering BooleanPrime() with token " + tokens.get(tokens.index())->getText());
        antlr4::Token *token = tokens.get(tokens.index());
        BooleanPrimeNode* result = nullptr;
        if (token->getType() == MITScript::OR) {
            tokens.consume();
            auto conjunction = Conjunction(tokens);
            auto rest = BooleanPrime(tokens);
            result = new BooleanPrimeNode(token, conjunction, rest);
        }
        if (result == nullptr){
            DEBUG_PRINT("Exiting BooleanPrime() with nullptr with token " + tokens.get(tokens.index())->getText());
        }else{
            DEBUG_PRINT("Exiting BooleanPrime() with token " + tokens.get(tokens.index())->getText());
        }
        return result;
    }

    BooleanNode* Boolean(antlr4::CommonTokenStream &tokens) {
        DEBUG_PRINT("Entering Boolean() with token " + tokens.get(tokens.index())->getText());
        auto conjunction = Conjunction(tokens);
        auto rest = BooleanPrime(tokens);
        BooleanNode* result = new BooleanNode(conjunction, rest);
        DEBUG_PRINT("Exiting Boolean() with token " + tokens.get(tokens.index())->getText());
        return result;
    }

    ConjunctionPrimeNode* ConjunctionPrime(antlr4::CommonTokenStream &tokens) {
        DEBUG_PRINT("Entering ConjunctionPrime() with token " + tokens.get(tokens.index())->getText());
        antlr4::Token *token = tokens.get(tokens.index());
        ConjunctionPrimeNode* result = nullptr;
        if (token->getType() == MITScript::AND) {
            tokens.consume();
            auto bool_unit = BoolUnit(tokens);
            auto rest = ConjunctionPrime(tokens);
            result = new ConjunctionPrimeNode(token, bool_unit, rest);
        }
        if (result == nullptr){
            DEBUG_PRINT("Exiting ConjunctionPrime() with nullptr with token " + tokens.get(tokens.index())->getText());
        }else{
            DEBUG_PRINT("Exiting ConjunctionPrime() with token " + tokens.get(tokens.index())->getText());
        }
        return result;
    }

    ConjunctionNode* Conjunction(antlr4::CommonTokenStream &tokens) {
        DEBUG_PRINT("Entering Conjunction() with token " + tokens.get(tokens.index())->getText());
        auto bool_unit = BoolUnit(tokens);
        auto rest = ConjunctionPrime(tokens);
        ConjunctionNode* result = new ConjunctionNode(bool_unit, rest);
        DEBUG_PRINT("Exiting Conjunction() with token " + tokens.get(tokens.index())->getText());
        return result;
    }

    BoolUnitNode* BoolUnit(antlr4::CommonTokenStream &tokens) {
        DEBUG_PRINT("Entering BoolUnit() with token " + tokens.get(tokens.index())->getText());
        antlr4::Token *token = tokens.get(tokens.index());
        BoolUnitNode* result = nullptr;
        if (token->getType() == MITScript::NOT) {
            tokens.consume();
            result = new BoolUnitNode(true, Predicate(tokens));
            DEBUG_PRINT("Exiting BoolUnit() with NOT with token " + tokens.get(tokens.index())->getText());
        }
        else{
            result = new BoolUnitNode(false, Predicate(tokens));
            DEBUG_PRINT("Exiting BoolUnit() with no NOT with token " + tokens.get(tokens.index())->getText());
        }
        return result;
    }

    PredicateNode* Predicate(antlr4::CommonTokenStream &tokens) {
        DEBUG_PRINT("Entering Predicate() with token " + tokens.get(tokens.index())->getText());
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
                DEBUG_PRINT("Exiting Predicate() with comparison operator with token " + tokens.get(tokens.index())->getText());
                return result;
            }
            default: {
                PredicateNode* result = new PredicateNode(arith_1, nullptr, nullptr);
                DEBUG_PRINT("Exiting Predicate() with no comparison operator with token " + tokens.get(tokens.index())->getText());
                return result;
            }
        }
        assert(0);  // This should ideally never be reached
    }
}
