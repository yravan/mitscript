/*
Written by me
*/
#include "nonterminals.h"
#include "CST.h"

namespace CST{

    BlockPrimeNode* BlockPrime(antlr4::CommonTokenStream &tokens) {

        antlr4::Token *token = tokens.get(tokens.index());
        if (token->getType() == MITScript::RBRACE || token->getType() == MITScript::EOF) {

            return nullptr;
        }
        auto statement = Statement(tokens);
        BlockPrimeNode* result = new BlockPrimeNode(statement, BlockPrime(tokens));

        return result;
    }

    BlockNode* Block(antlr4::CommonTokenStream &tokens) {

        antlr4::Token *token = tokens.get(tokens.index());
        if (token->getType() == MITScript::LBRACE) {
            tokens.consume();
            auto inside = BlockPrime(tokens);
            antlr4::Token *next_token = tokens.get(tokens.index());
            if (next_token->getType() == MITScript::RBRACE) {
                tokens.consume();

                return new BlockNode(inside);
            }
            reportError(*next_token);
        }
        reportError(*token);
        return nullptr;
    }

    ArgumentPrimeNode* ArgumentPrime(antlr4::CommonTokenStream &tokens) {

        antlr4::Token *token = tokens.get(tokens.index());
        if (token->getType() == MITScript::COMMA) {
            tokens.consume();
            antlr4::Token *token_2 = tokens.get(tokens.index());
            if (token_2->getType() == MITScript::NAME) {
                tokens.consume();
                ArgumentPrimeNode* result = new ArgumentPrimeNode(token_2, ArgumentPrime(tokens));

                return result;
            }
            reportError(*token_2);
        }

        return nullptr;
    }

    ArgumentNode* Argument(antlr4::CommonTokenStream &tokens) {

        antlr4::Token *token = tokens.get(tokens.index());
        if (token->getType() == MITScript::RPAREN) {

            return nullptr;
        }
        if (token->getType() == MITScript::NAME) {
            tokens.consume();
            ArgumentNode* result = new ArgumentNode(token, ArgumentPrime(tokens));

            return result;
        }
        reportError(*token);
        return nullptr;
    }

    FunctionNode* Function(antlr4::CommonTokenStream &tokens) {

        antlr4::Token *token = tokens.get(tokens.index());
        if (token->getType() == MITScript::FUN) {
            tokens.consume();
            antlr4::Token *token_2 = tokens.get(tokens.index());
            if (token_2->getType() == MITScript::LPAREN) {
                tokens.consume();
                auto args = Argument(tokens);
                antlr4::Token *token_3 = tokens.get(tokens.index());
                if (token_3->getType() == MITScript::RPAREN) {
                    tokens.consume();
                    FunctionNode* result = new FunctionNode(args, Block(tokens));

                    return result;
                }
                reportError(*token_3);
            }
            reportError(*token_2);
        }
        reportError(*token);
        return nullptr;
    }

    WhileNode* While(antlr4::CommonTokenStream &tokens) {

        antlr4::Token *token = tokens.get(tokens.index());
        if (token->getType() == MITScript::WHILE) {
            tokens.consume();
            antlr4::Token *token_2 = tokens.get(tokens.index());
            if (token_2->getType() == MITScript::LPAREN) {
                tokens.consume();
                auto expr = Expression(tokens);
                antlr4::Token *token_3 = tokens.get(tokens.index());
                if (token_3->getType() == MITScript::RPAREN) {
                    tokens.consume();
                    WhileNode* result = new WhileNode(expr, Block(tokens));

                    return result;
                }
                reportError(*token_3);
            }
            reportError(*token_2);
        }
        reportError(*token);
        return nullptr;
    }

    ElseNode* Else(antlr4::CommonTokenStream &tokens) {

        antlr4::Token *token = tokens.get(tokens.index());
        if (token->getType() == MITScript::ELSE) {
            tokens.consume();
            ElseNode* result = new ElseNode(Block(tokens));

            return result;
        }

        return nullptr;
    }

    IfStatementNode* IfStatement(antlr4::CommonTokenStream &tokens) {

        antlr4::Token *token = tokens.get(tokens.index());
        if (token->getType() == MITScript::IF) {
            tokens.consume();
            antlr4::Token *token_2 = tokens.get(tokens.index());
            if (token_2->getType() == MITScript::LPAREN) {
                tokens.consume();
                auto expr = Expression(tokens);
                antlr4::Token *token_3 = tokens.get(tokens.index());
                if (token_3->getType() == MITScript::RPAREN) {
                    tokens.consume();
                    auto block = Block(tokens);
                    auto else_block = Else(tokens);
                    IfStatementNode* result = new IfStatementNode(expr, block, else_block);

                    return result;
                }
                reportError(*token_3);
            }
            reportError(*token_2);
        }
        reportError(*token);
        return nullptr;
    }

    StatementNode* Statement(antlr4::CommonTokenStream &tokens) {

        antlr4::Token *token = tokens.get(tokens.index());
        switch (token->getType()) {
            case MITScript::GLOBAL:

                return new StatementNode(nullptr, nullptr, Global(tokens), nullptr, nullptr, nullptr);
            case MITScript::IF:

                return new StatementNode(nullptr, nullptr, nullptr, IfStatement(tokens), nullptr, nullptr);
            case MITScript::WHILE:

                return new StatementNode(nullptr, nullptr, nullptr, nullptr, While(tokens), nullptr);
            case MITScript::RETURN:

                return new StatementNode(nullptr, nullptr, nullptr, nullptr, nullptr, Return(tokens));
        }
        int cur_token_index = tokens.index();
        auto lhs = LHS(tokens);
        token = tokens.get(tokens.index());
        switch (token->getType()) {
            case MITScript::ASSIGN:
                tokens.seek(cur_token_index);

                return new StatementNode(Assignment(tokens), nullptr, nullptr, nullptr, nullptr, nullptr);
            case MITScript::LPAREN:
                tokens.seek(cur_token_index);

                return new StatementNode(nullptr, CallStatement(tokens), nullptr, nullptr, nullptr, nullptr);
        }
        reportError(*token);
        return nullptr;
    }

    StatementListNode* StatementList(antlr4::CommonTokenStream &tokens) {

        antlr4::Token *token = tokens.get(tokens.index());
        if (token->getType() == MITScript::EOF) {

            return nullptr;
        }
        auto statement = Statement(tokens);
        StatementListNode* result = new StatementListNode(statement, StatementList(tokens));

        return result;
    }

    ProgramNode* Program(antlr4::CommonTokenStream &tokens) {


        StatementListNode* result = StatementList(tokens);
        antlr4::Token *last_token = tokens.get(tokens.index());
        if (last_token->getType() != MITScript::EOF) {
            reportError(*last_token);
        }

        return new ProgramNode(result);
    }
}