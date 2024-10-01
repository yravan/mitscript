#include "nonterminals.h"
#include "CST.h"

BlockPrimeNode* BlockPrime(antlr4::CommonTokenStream &tokens) {
#ifdef DEBUG
    std::cout << "Entering BlockPrime()" << "with token " << tokens.get(tokens.index())->getText() << std::endl ;
#endif
    antlr4::Token *token = tokens.get(tokens.index());
    if (token->getType() == MITScript::RBRACE) {
#ifdef DEBUG
        std::cout << "Exiting BlockPrime() with nullptr" << "with token " << tokens.get(tokens.index())->getText() << std::endl ;
#endif
        return nullptr;
    }
    if (token->getType() == MITScript::EOF) {
#ifdef DEBUG
        std::cout << "Exiting BlockPrime() with nullptr (EOF)" << "with token " << tokens.get(tokens.index())->getText() << std::endl ;
#endif
        return nullptr;
    }
    auto statement = Statement(tokens);
    BlockPrimeNode* result = new BlockPrimeNode(statement, BlockPrime(tokens));
#ifdef DEBUG
    std::cout << "Exiting BlockPrime()" << "with token " << tokens.get(tokens.index())->getText() << std::endl ;
#endif
    return result;
}

BlockNode* Block(antlr4::CommonTokenStream &tokens) {
#ifdef DEBUG
    std::cout << "Entering Block()" << "with token " << tokens.get(tokens.index())->getText() << std::endl ;
#endif
    antlr4::Token *token = tokens.get(tokens.index());
    if (token->getType() == MITScript::LBRACE) {
        tokens.consume();
        auto inside = BlockPrime(tokens);
        antlr4::Token *next_token = tokens.get(tokens.index());
        if (next_token->getType() == MITScript::RBRACE) {
            tokens.consume();
#ifdef DEBUG
            std::cout << "Exiting Block()" << "with token " << tokens.get(tokens.index())->getText() << std::endl ;
#endif
            return new BlockNode(inside);
        }
        reportError(*next_token);
    }
    reportError(*token);
}

ArgumentPrimeNode* ArgumentPrime(antlr4::CommonTokenStream &tokens) {
#ifdef DEBUG
    std::cout << "Entering ArgumentPrime()" << "with token " << tokens.get(tokens.index())->getText() << std::endl ;
#endif
    antlr4::Token *token = tokens.get(tokens.index());
    if (token->getType() == MITScript::COMMA) {
        tokens.consume();
        antlr4::Token *token_2 = tokens.get(tokens.index());
        if (token_2->getType() == MITScript::NAME) {
            tokens.consume();
            ArgumentPrimeNode* result = new ArgumentPrimeNode(token_2, ArgumentPrime(tokens));
#ifdef DEBUG
            std::cout << "Exiting ArgumentPrime()" << "with token " << tokens.get(tokens.index())->getText() << std::endl ;
#endif
            return result;
        }
        reportError(*token_2);
    }
#ifdef DEBUG
    std::cout << "Exiting ArgumentPrime() with nullptr" << "with token " << tokens.get(tokens.index())->getText() << std::endl ;
#endif
    return nullptr;
}

ArgumentNode* Argument(antlr4::CommonTokenStream &tokens) {
#ifdef DEBUG
    std::cout << "Entering Argument()" << "with token " << tokens.get(tokens.index())->getText() << std::endl ;
#endif
    antlr4::Token *token = tokens.get(tokens.index());
    if (token->getType() == MITScript::RPAREN) {
#ifdef DEBUG
        std::cout << "Exiting Argument() with nullptr (RPAREN)" << "with token " << tokens.get(tokens.index())->getText() << std::endl ;
#endif
        return nullptr;
    }
    if (token->getType() == MITScript::NAME) {
        tokens.consume();
        ArgumentNode* result = new ArgumentNode(token, ArgumentPrime(tokens));
#ifdef DEBUG
        std::cout << "Exiting Argument()" << "with token " << tokens.get(tokens.index())->getText() << std::endl ;
#endif
        return result;
    }
    reportError(*token);
}

FunctionNode* Function(antlr4::CommonTokenStream &tokens) {
#ifdef DEBUG
    std::cout << "Entering Function()" << "with token " << tokens.get(tokens.index())->getText() << std::endl ;
#endif
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
#ifdef DEBUG
                std::cout << "Exiting Function()" << "with token " << tokens.get(tokens.index())->getText() << std::endl ;
#endif
                return result;
            }
            reportError(*token_3);
        }
        reportError(*token_2);
    }
    reportError(*token);
}

WhileNode* While(antlr4::CommonTokenStream &tokens) {
#ifdef DEBUG
    std::cout << "Entering While()" << "with token " << tokens.get(tokens.index())->getText() << std::endl ;
#endif
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
#ifdef DEBUG
                std::cout << "Exiting While()" << "with token " << tokens.get(tokens.index())->getText() << std::endl ;
#endif
                return result;
            }
            reportError(*token_3);
        }
        reportError(*token_2);
    }
    reportError(*token);
}

ElseNode* Else(antlr4::CommonTokenStream &tokens) {
#ifdef DEBUG
    std::cout << "Entering Else()" << "with token " << tokens.get(tokens.index())->getText() << std::endl ;
#endif
    antlr4::Token *token = tokens.get(tokens.index());
    if (token->getType() == MITScript::ELSE) {
        tokens.consume();
        ElseNode* result = new ElseNode(Block(tokens));
#ifdef DEBUG
        std::cout << "Exiting Else()" << "with token " << tokens.get(tokens.index())->getText() << std::endl ;
#endif
        return result;
    }
#ifdef DEBUG
    std::cout << "Exiting Else() with nullptr" << "with token " << tokens.get(tokens.index())->getText() << std::endl ;
#endif
    return nullptr;
}

IfStatementNode* IfStatement(antlr4::CommonTokenStream &tokens) {
#ifdef DEBUG
    std::cout << "Entering IfStatement()" << "with token " << tokens.get(tokens.index())->getText() << std::endl ;
#endif
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
#ifdef DEBUG
                std::cout << "Exiting IfStatement()" << "with token " << tokens.get(tokens.index())->getText() << std::endl ;
#endif
                return result;
            }
            reportError(*token_3);
        }
        reportError(*token_2);
    }
    reportError(*token);
}

StatementNode* Statement(antlr4::CommonTokenStream &tokens) {
#ifdef DEBUG
    std::cout << "Entering Statement()" << "with token " << tokens.get(tokens.index())->getText() << std::endl ;
#endif
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
}


StatementListNode* StatementList(antlr4::CommonTokenStream &tokens) {
#ifdef DEBUG
    std::cout << "Entering StatementList()" << "with token " << tokens.get(tokens.index())->getText() << std::endl ;
#endif
    antlr4::Token *token = tokens.get(tokens.index());
    if (token->getType() == MITScript::EOF) {
#ifdef DEBUG
        std::cout << "Exiting StatementList() with nullptr (EOF)" << "with token " << tokens.get(tokens.index())->getText() << std::endl ;
#endif
        return nullptr;
    }
    auto statement = Statement(tokens);
    StatementListNode* result = new StatementListNode(statement, StatementList(tokens));
#ifdef DEBUG
    std::cout << "Exiting StatementList()" << "with token " << tokens.get(tokens.index())->getText() << std::endl ;
#endif
    return result;
}
