/*
Written by me
*/
#include "nonterminals.h"
#include "CST.h"

namespace CST{
    RecordPrimeNode* RecordPrime(antlr4::CommonTokenStream &tokens) {

        antlr4::Token *token = tokens.get(tokens.index());
        if (token->getType() == MITScript::NAME) {
            tokens.consume();
            antlr4::Token *token_2 = tokens.get(tokens.index());
            if (token_2->getType() == MITScript::COLON) {
                tokens.consume();
                auto expr = Expression(tokens);
                antlr4::Token *token_3 = tokens.get(tokens.index());
                if (token_3->getType() == MITScript::SEMICOLON) {
                    tokens.consume();
                    RecordPrimeNode* result = new RecordPrimeNode(token, expr, RecordPrime(tokens));

                    return result;
                }
                reportError(*token_3);
            }
        }
        if (token->getType() == MITScript::RBRACE) {

            return nullptr;
        }
        reportError(*token);
        return nullptr;
    }

    RecordNode* Record(antlr4::CommonTokenStream &tokens) {

        antlr4::Token *token = tokens.get(tokens.index());
        if (token->getType() == MITScript::LBRACE) {
            tokens.consume();
            auto inside = RecordPrime(tokens);
            antlr4::Token *next_token = tokens.get(tokens.index());
            if (next_token->getType() == MITScript::RBRACE) {
                tokens.consume();
                RecordNode* result = new RecordNode(inside);

                return result;
            }
            reportError(*next_token);
        }
        reportError(*token);
        return nullptr;
    }

    LHSPrimeNode* LHSPrime(antlr4::CommonTokenStream &tokens) {

        antlr4::Token *token = tokens.get(tokens.index());
        if (token->getType() == MITScript::DOT) {
            tokens.consume();
            antlr4::Token *token_2 = tokens.get(tokens.index());
            if (token_2->getType() == MITScript::NAME) {
                tokens.consume();
                LHSPrimeNode* result = new LHSPrimeNode(token_2, nullptr, LHSPrime(tokens));

                return result;
            }
            reportError(*token_2);
        }
        if (token->getType() == MITScript::LBRACKET) {
            tokens.consume();
            auto expr = Expression(tokens);
            antlr4::Token *token_2 = tokens.get(tokens.index());
            if (token_2->getType() == MITScript::RBRACKET) {
                tokens.consume();
                LHSPrimeNode* result = new LHSPrimeNode(nullptr, expr, LHSPrime(tokens));

                return result;
            }
            reportError(*token_2);
        }

        return nullptr;
    }

    LHSNode* LHS(antlr4::CommonTokenStream &tokens) {

        antlr4::Token *token = tokens.get(tokens.index());
        if (token->getType() == MITScript::NAME) {
            tokens.consume();
            LHSNode* result = new LHSNode(token, LHSPrime(tokens));

            return result;
        }
        reportError(*token);
        return nullptr;
    }

    ReturnNode* Return(antlr4::CommonTokenStream &tokens) {

        antlr4::Token *token = tokens.get(tokens.index());
        if (token->getType() == MITScript::RETURN) {
            tokens.consume();
            auto expr = Expression(tokens);
            antlr4::Token *token_2 = tokens.get(tokens.index());
            if (token_2->getType() == MITScript::SEMICOLON) {
                tokens.consume();
                ReturnNode* result = new ReturnNode(expr);

                return result;
            }
            reportError(*token_2);
        }
        reportError(*token);
        return nullptr;
    }

    ExpressionNode* Expression(antlr4::CommonTokenStream &tokens) { // NOT fully implemented

        antlr4::Token *token = tokens.get(tokens.index());
        ExpressionNode* result = nullptr;

        switch (token->getType()) {
            case MITScript::LBRACE: {
                result = new ExpressionNode(nullptr, nullptr, Record(tokens));
                break;
            }
            case MITScript::FUN: {
                result = new ExpressionNode(Function(tokens), nullptr, nullptr);
                break;
            }
            default: {
                result = new ExpressionNode(nullptr, Boolean(tokens), nullptr);
            }
        }

        return result;
    }

    ParameterPrimeNode* ParameterPrime(antlr4::CommonTokenStream &tokens) {

        antlr4::Token *token = tokens.get(tokens.index());
        if (token->getType() == MITScript::COMMA) {
            tokens.consume();
            auto expr = Expression(tokens);
            auto rest = ParameterPrime(tokens);
            ParameterPrimeNode* result = new ParameterPrimeNode(expr, rest);

            return result;
        }

        return nullptr;
    }

    ParameterNode* Parameter(antlr4::CommonTokenStream &tokens) {

        antlr4::Token *token = tokens.get(tokens.index());
        if (token->getType() == MITScript::RPAREN) {

            return nullptr;
        }
        auto expr = Expression(tokens);
        auto rest = ParameterPrime(tokens);
        ParameterNode* result = new ParameterNode(expr, rest);

        return result;
    }

    CallNode* Call(antlr4::CommonTokenStream &tokens) {

        auto lhs = LHS(tokens);
        antlr4::Token *token = tokens.get(tokens.index());
        if (token->getType() == MITScript::LPAREN) {
            tokens.consume();
            auto args = Parameter(tokens);
            antlr4::Token *token_2 = tokens.get(tokens.index());
            if (token_2->getType() == MITScript::RPAREN) {
                tokens.consume();
                CallNode* result = new CallNode(lhs, args);

                return result;
            }
            reportError(*token_2);
        }
        reportError(*token);
        return nullptr;
    }

    CallStatementNode* CallStatement(antlr4::CommonTokenStream &tokens) {

        auto call = Call(tokens);
        antlr4::Token *token = tokens.get(tokens.index());
        if (token->getType() == MITScript::SEMICOLON) {
            tokens.consume();
            CallStatementNode* result = new CallStatementNode(call);

            return result;
        }
        reportError(*token);
        return nullptr;
    }

    AssignmentNode* Assignment(antlr4::CommonTokenStream &tokens) {

        auto lhs = LHS(tokens);
        antlr4::Token *token = tokens.get(tokens.index());
        if (token->getType() == MITScript::ASSIGN) {
            tokens.consume();
            auto expr = Expression(tokens);
            antlr4::Token *token_2 = tokens.get(tokens.index());
            if (token_2->getType() == MITScript::SEMICOLON) {
                tokens.consume();
                AssignmentNode* result = new AssignmentNode(lhs, expr);

                return result;
            }
            reportError(*token_2);
        }
        reportError(*token);
        return nullptr;
    }

    GlobalNode* Global(antlr4::CommonTokenStream &tokens) {

        antlr4::Token *token = tokens.get(tokens.index());
        if (token->getType() == MITScript::GLOBAL) {
            tokens.consume();
            antlr4::Token *nextToken = tokens.get(tokens.index());
            if (nextToken->getType() == MITScript::NAME) {
                tokens.consume();
                antlr4::Token *thirdToken = tokens.get(tokens.index());
                if (thirdToken->getType() == MITScript::SEMICOLON) {
                    tokens.consume();
                    GlobalNode* result = new GlobalNode(nextToken);

                    return result;
                }
                reportError(*thirdToken);
            }
            reportError(*nextToken);
        }
        reportError(*token);
        return nullptr;
    }

    UnitNode* Unit(antlr4::CommonTokenStream &tokens) {

        antlr4::Token *token = tokens.get(tokens.index());
        bool minus = false;

        if (token->getType() == MITScript::MINUS) {
            tokens.consume();
            minus = true;
            token = tokens.get(tokens.index());
        }
        if (token->getType() == MITScript::LPAREN) {
            tokens.consume();
            auto boolean = Boolean(tokens);
            antlr4::Token *token = tokens.get(tokens.index());
            if (token->getType() == MITScript::RPAREN){
                tokens.consume();

                return new UnitNode(minus, nullptr, nullptr, nullptr, boolean);
            }
            reportError(*token);
            return nullptr;
        }
        if (token->getType() == MITScript::NAME) {
            int cur_token_index = tokens.index();
            auto lhs = LHS(tokens);
            token = tokens.get(tokens.index());
            if (token->getType() == MITScript::LPAREN) {
                tokens.seek(cur_token_index);

                return new UnitNode(minus, nullptr, nullptr, Call(tokens), nullptr);
            }

            return new UnitNode(minus, lhs, nullptr, nullptr, nullptr);
        }


        UnitNode* result = new UnitNode(minus, nullptr, Constant(tokens), nullptr, nullptr);
        return result;
    }
}