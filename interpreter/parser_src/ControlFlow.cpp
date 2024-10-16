/*
Written by me
*/
#include "nonterminals.h"
#include "CST.h"

namespace CST{

    BlockPrimeNode* BlockPrime(antlr4::CommonTokenStream &tokens) {
        DEBUG_PRINT("Entering BlockPrime() with token " + tokens.get(tokens.index())->getText());
        antlr4::Token *token = tokens.get(tokens.index());
        if (token->getType() == MITScript::RBRACE || token->getType() == MITScript::EOF) {
            DEBUG_PRINT("Exiting BlockPrime() with nullptr with token " + tokens.get(tokens.index())->getText());
            return nullptr;
        }
        auto statement = Statement(tokens);
        BlockPrimeNode* result = new BlockPrimeNode(statement, BlockPrime(tokens));
        DEBUG_PRINT("Exiting BlockPrime() with token " + tokens.get(tokens.index())->getText());
        return result;
    }

    BlockNode* Block(antlr4::CommonTokenStream &tokens) {
        DEBUG_PRINT("Entering Block() with token " + tokens.get(tokens.index())->getText());
        antlr4::Token *token = tokens.get(tokens.index());
        if (token->getType() == MITScript::LBRACE) {
            tokens.consume();
            auto inside = BlockPrime(tokens);
            antlr4::Token *next_token = tokens.get(tokens.index());
            if (next_token->getType() == MITScript::RBRACE) {
                tokens.consume();
                DEBUG_PRINT("Exiting Block() with token " + tokens.get(tokens.index())->getText());
                return new BlockNode(inside);
            }
            reportError(*next_token);
        }
        reportError(*token);
        return nullptr;
    }

    ArgumentPrimeNode* ArgumentPrime(antlr4::CommonTokenStream &tokens) {
        DEBUG_PRINT("Entering ArgumentPrime() with token " + tokens.get(tokens.index())->getText());
        antlr4::Token *token = tokens.get(tokens.index());
        if (token->getType() == MITScript::COMMA) {
            tokens.consume();
            antlr4::Token *token_2 = tokens.get(tokens.index());
            if (token_2->getType() == MITScript::NAME) {
                tokens.consume();
                ArgumentPrimeNode* result = new ArgumentPrimeNode(token_2, ArgumentPrime(tokens));
                DEBUG_PRINT("Exiting ArgumentPrime() with token " + tokens.get(tokens.index())->getText());
                return result;
            }
            reportError(*token_2);
        }
        DEBUG_PRINT("Exiting ArgumentPrime() with nullptr with token " + tokens.get(tokens.index())->getText());
        return nullptr;
    }

    ArgumentNode* Argument(antlr4::CommonTokenStream &tokens) {
        DEBUG_PRINT("Entering Argument() with token " + tokens.get(tokens.index())->getText());
        antlr4::Token *token = tokens.get(tokens.index());
        if (token->getType() == MITScript::RPAREN) {
            DEBUG_PRINT("Exiting Argument() with nullptr (RPAREN) with token " + tokens.get(tokens.index())->getText());
            return nullptr;
        }
        if (token->getType() == MITScript::NAME) {
            tokens.consume();
            ArgumentNode* result = new ArgumentNode(token, ArgumentPrime(tokens));
            DEBUG_PRINT("Exiting Argument() with token " + tokens.get(tokens.index())->getText());
            return result;
        }
        reportError(*token);
        return nullptr;
    }

    FunctionNode* Function(antlr4::CommonTokenStream &tokens) {
        DEBUG_PRINT("Entering Function() with token " + tokens.get(tokens.index())->getText());
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
                    DEBUG_PRINT("Exiting Function() with token " + tokens.get(tokens.index())->getText());
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
        DEBUG_PRINT("Entering While() with token " + tokens.get(tokens.index())->getText());
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
                    DEBUG_PRINT("Exiting While() with token " + tokens.get(tokens.index())->getText());
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
        DEBUG_PRINT("Entering Else() with token " + tokens.get(tokens.index())->getText());
        antlr4::Token *token = tokens.get(tokens.index());
        if (token->getType() == MITScript::ELSE) {
            tokens.consume();
            ElseNode* result = new ElseNode(Block(tokens));
            DEBUG_PRINT("Exiting Else() with token " + tokens.get(tokens.index())->getText());
            return result;
        }
        DEBUG_PRINT("Exiting Else() with nullptr with token " + tokens.get(tokens.index())->getText());
        return nullptr;
    }

    IfStatementNode* IfStatement(antlr4::CommonTokenStream &tokens) {
        DEBUG_PRINT("Entering IfStatement() with token " + tokens.get(tokens.index())->getText());
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
                    DEBUG_PRINT("Exiting IfStatement() with token " + tokens.get(tokens.index())->getText());
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
        DEBUG_PRINT("Entering Statement() with token " + tokens.get(tokens.index())->getText());
        antlr4::Token *token = tokens.get(tokens.index());
        switch (token->getType()) {
            case MITScript::GLOBAL:
                DEBUG_PRINT("Exiting Statement() with token " + tokens.get(tokens.index())->getText());
                return new StatementNode(nullptr, nullptr, Global(tokens), nullptr, nullptr, nullptr);
            case MITScript::IF:
                DEBUG_PRINT("Exiting Statement() with token " + tokens.get(tokens.index())->getText());
                return new StatementNode(nullptr, nullptr, nullptr, IfStatement(tokens), nullptr, nullptr);
            case MITScript::WHILE:
                DEBUG_PRINT("Exiting Statement() with token " + tokens.get(tokens.index())->getText());
                return new StatementNode(nullptr, nullptr, nullptr, nullptr, While(tokens), nullptr);
            case MITScript::RETURN:
                DEBUG_PRINT("Exiting Statement() with token " + tokens.get(tokens.index())->getText());
                return new StatementNode(nullptr, nullptr, nullptr, nullptr, nullptr, Return(tokens));
        }
        int cur_token_index = tokens.index();
        auto lhs = LHS(tokens);
        token = tokens.get(tokens.index());
        switch (token->getType()) {
            case MITScript::ASSIGN:
                tokens.seek(cur_token_index);
                DEBUG_PRINT("Exiting Statement() with token " + tokens.get(tokens.index())->getText());
                return new StatementNode(Assignment(tokens), nullptr, nullptr, nullptr, nullptr, nullptr);
            case MITScript::LPAREN:
                tokens.seek(cur_token_index);
                DEBUG_PRINT("Exiting Statement() with token " + tokens.get(tokens.index())->getText());
                return new StatementNode(nullptr, CallStatement(tokens), nullptr, nullptr, nullptr, nullptr);
        }
        reportError(*token);
        return nullptr;
    }

    StatementListNode* StatementList(antlr4::CommonTokenStream &tokens) {
        DEBUG_PRINT("Entering StatementList() with token " + tokens.get(tokens.index())->getText());
        antlr4::Token *token = tokens.get(tokens.index());
        if (token->getType() == MITScript::EOF) {
            DEBUG_PRINT("Exiting StatementList() with nullptr (EOF) with token " + tokens.get(tokens.index())->getText());
            return nullptr;
        }
        auto statement = Statement(tokens);
        StatementListNode* result = new StatementListNode(statement, StatementList(tokens));
        DEBUG_PRINT("Exiting StatementList() with token " + tokens.get(tokens.index())->getText());
        return result;
    }

    ProgramNode* Program(antlr4::CommonTokenStream &tokens) {
        DEBUG_PRINT("Entering Program() with token " + tokens.get(tokens.index())->getText());

        StatementListNode* result = StatementList(tokens);
        antlr4::Token *last_token = tokens.get(tokens.index());
        if (last_token->getType() != MITScript::EOF) {
            reportError(*last_token);
        }
        DEBUG_PRINT("Exiting Program()");
        return new ProgramNode(result);
    }
}