#include "nonterminals.h"
#include "CST.h"

RecordPrimeNode* RecordPrime(antlr4::CommonTokenStream &tokens) {
#ifdef DEBUG
    std::cout << "Entering RecordPrime()" << "with token " << tokens.get(tokens.index())->getText() << std::endl ;
#endif
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
#ifdef DEBUG
                std::cout << "Exiting RecordPrime()" << "with token " << tokens.get(tokens.index())->getText() << std::endl ;
#endif
                return result;
            }
            reportError(*token_3);
        }
    }
    if (token->getType() == MITScript::RBRACE) {
#ifdef DEBUG
        std::cout << "Exiting RecordPrime() with nullptr" << "with token " << tokens.get(tokens.index())->getText() << std::endl ;
#endif
        return nullptr;
    }
    reportError(*token);
}

RecordNode* Record(antlr4::CommonTokenStream &tokens) {
#ifdef DEBUG
    std::cout << "Entering Record()" << "with token " << tokens.get(tokens.index())->getText() << std::endl ;
#endif
    antlr4::Token *token = tokens.get(tokens.index());
    if (token->getType() == MITScript::LBRACE) {
        tokens.consume();
        auto inside = RecordPrime(tokens);
        antlr4::Token *next_token = tokens.get(tokens.index());
        if (next_token->getType() == MITScript::RBRACE) {
            tokens.consume();
            RecordNode* result = new RecordNode(inside);
#ifdef DEBUG
            std::cout << "Exiting Record()" << "with token " << tokens.get(tokens.index())->getText() << std::endl ;
#endif
            return result;
        }
        reportError(*next_token);
    }
    reportError(*token);
}

LHSPrimeNode* LHSPrime(antlr4::CommonTokenStream &tokens) {
#ifdef DEBUG
    std::cout << "Entering LHSPrime()" << "with token " << tokens.get(tokens.index())->getText() << std::endl ;
#endif
    antlr4::Token *token = tokens.get(tokens.index());
    if (token->getType() == MITScript::DOT) {
        tokens.consume();
        antlr4::Token *token_2 = tokens.get(tokens.index());
        if (token_2->getType() == MITScript::NAME) {
            tokens.consume();
            LHSPrimeNode* result = new LHSPrimeNode(token_2, nullptr, LHSPrime(tokens));
#ifdef DEBUG
            std::cout << "Exiting LHSPrime()" << "with token " << tokens.get(tokens.index())->getText() << std::endl ;
#endif
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
#ifdef DEBUG
            std::cout << "Exiting LHSPrime()" << "with token " << tokens.get(tokens.index())->getText() << std::endl ;
#endif
            return result;
        }
        reportError(*token_2);
    }
#ifdef DEBUG
    std::cout << "Exiting LHSPrime() with nullptr" << "with token " << tokens.get(tokens.index())->getText() << std::endl ;
#endif
    return nullptr;
}

LHSNode* LHS(antlr4::CommonTokenStream &tokens) {
#ifdef DEBUG
    std::cout << "Entering LHS()" << "with token " << tokens.get(tokens.index())->getText() << std::endl ;
#endif
    antlr4::Token *token = tokens.get(tokens.index());
    if (token->getType() == MITScript::NAME) {
        tokens.consume();
        LHSNode* result = new LHSNode(token, LHSPrime(tokens));
#ifdef DEBUG
        std::cout << "Exiting LHS()" << "with token " << tokens.get(tokens.index())->getText() << std::endl ;
#endif
        return result;
    }
    reportError(*token);
}

ReturnNode* Return(antlr4::CommonTokenStream &tokens) {
#ifdef DEBUG
    std::cout << "Entering Return()" << "with token " << tokens.get(tokens.index())->getText() << std::endl ;
#endif
    antlr4::Token *token = tokens.get(tokens.index());
    if (token->getType() == MITScript::RETURN) {
        tokens.consume();
        auto expr = Expression(tokens);
        antlr4::Token *token_2 = tokens.get(tokens.index());
        if (token_2->getType() == MITScript::SEMICOLON) {
            tokens.consume();
            ReturnNode* result = new ReturnNode(expr);
#ifdef DEBUG
            std::cout << "Exiting Return()" << "with token " << tokens.get(tokens.index())->getText() << std::endl ;
#endif
            return result;
        }
        reportError(*token_2);
    }
    reportError(*token);
}

ExpressionNode* Expression(antlr4::CommonTokenStream &tokens) { // NOT fully implemented
#ifdef DEBUG
    std::cout << "Entering Expression()" << "with token " << tokens.get(tokens.index())->getText() << std::endl ;
#endif
    antlr4::Token *token = tokens.get(tokens.index());
    
    switch (token->getType()) {
        case MITScript::LBRACE:
            {
                ExpressionNode* result = new ExpressionNode(nullptr, nullptr, Record(tokens));
#ifdef DEBUG
                std::cout << "Exiting Expression()" << "with token " << tokens.get(tokens.index())->getText() << std::endl ;
#endif
                return result;
            }
        case MITScript::FUN:
            {
                ExpressionNode* result = new ExpressionNode(Function(tokens), nullptr, nullptr);
#ifdef DEBUG
                std::cout << "Exiting Expression()" << "with token " << tokens.get(tokens.index())->getText() << std::endl ;
#endif
                return result;
            }
        default:
            {
                ExpressionNode* result = new ExpressionNode(nullptr, Boolean(tokens), nullptr);
#ifdef DEBUG
                std::cout << "Exiting Expression()" << "with token " << tokens.get(tokens.index())->getText() << std::endl ;
#endif
                return result;
            }
    }
    assert(0);
}

ParameterPrimeNode* ParameterPrime(antlr4::CommonTokenStream &tokens) {
#ifdef DEBUG
    std::cout << "Entering ParameterPrime()" << "with token " << tokens.get(tokens.index())->getText() << std::endl ;
#endif
    antlr4::Token *token = tokens.get(tokens.index());
    if (token->getType() == MITScript::COMMA) {
        tokens.consume();
        ParameterPrimeNode* result = new ParameterPrimeNode(Expression(tokens), ParameterPrime(tokens));
#ifdef DEBUG
        std::cout << "Exiting ParameterPrime()" << "with token " << tokens.get(tokens.index())->getText() << std::endl ;
#endif
        return result;
    }
#ifdef DEBUG
    std::cout << "Exiting ParameterPrime() with nullptr" << "with token " << tokens.get(tokens.index())->getText() << std::endl ;
#endif
    return nullptr;
}

ParameterNode* Parameter(antlr4::CommonTokenStream &tokens) {
#ifdef DEBUG
    std::cout << "Entering Parameter()" << "with token " << tokens.get(tokens.index())->getText() << std::endl ;
#endif
    antlr4::Token *token = tokens.get(tokens.index());
    if (token->getType() == MITScript::RPAREN) {
        return nullptr;
    }
    ParameterNode* result = new ParameterNode(Expression(tokens), ParameterPrime(tokens));
#ifdef DEBUG
    std::cout << "Exiting Parameter()" << "with token " << tokens.get(tokens.index())->getText() << std::endl ;
#endif
    return result;
}

CallNode* Call(antlr4::CommonTokenStream &tokens) {
#ifdef DEBUG
    std::cout << "Entering Call()" << "with token " << tokens.get(tokens.index())->getText() << std::endl ;
#endif
    auto lhs = LHS(tokens);
    antlr4::Token *token = tokens.get(tokens.index());
    if (token->getType() == MITScript::LPAREN) {
        tokens.consume();
        auto args = Parameter(tokens);
        antlr4::Token *token_2 = tokens.get(tokens.index());
        if (token_2->getType() == MITScript::RPAREN) {
            tokens.consume();
            CallNode* result = new CallNode(lhs, args);
#ifdef DEBUG
            std::cout << "Exiting Call()" << "with token " << tokens.get(tokens.index())->getText() << std::endl ;
#endif
            return result;
        }
        reportError(*token_2);
    }
    reportError(*token);
}

CallStatementNode* CallStatement(antlr4::CommonTokenStream &tokens) {
#ifdef DEBUG
    std::cout << "Entering CallStatement()" << "with token " << tokens.get(tokens.index())->getText() << std::endl ;
#endif
    auto call = Call(tokens);
    antlr4::Token *token = tokens.get(tokens.index());
    if (token->getType() == MITScript::SEMICOLON) {
        tokens.consume();
        CallStatementNode* result = new CallStatementNode(call);
#ifdef DEBUG
        std::cout << "Exiting CallStatement()" << "with token " << tokens.get(tokens.index())->getText() << std::endl ;
#endif
        return result;
    }
    reportError(*token);
}

AssignmentNode* Assignment(antlr4::CommonTokenStream &tokens) {
#ifdef DEBUG
    std::cout << "Entering Assignment()" << "with token " << tokens.get(tokens.index())->getText() << std::endl ;
#endif
    auto lhs = LHS(tokens);
    antlr4::Token *token = tokens.get(tokens.index());
    if (token->getType() == MITScript::ASSIGN) {
        tokens.consume();
        auto expr = Expression(tokens);
        antlr4::Token *token_2 = tokens.get(tokens.index());
        if (token_2->getType() == MITScript::SEMICOLON) {
            tokens.consume();
            AssignmentNode* result = new AssignmentNode(lhs, expr);
#ifdef DEBUG
            std::cout << "Exiting Assignment()" << "with token " << tokens.get(tokens.index())->getText() << std::endl ;
#endif
            return result;
        }
        reportError(*token_2);
    }
    reportError(*token);
}

GlobalNode* Global(antlr4::CommonTokenStream &tokens) {
#ifdef DEBUG
    std::cout << "Entering Global()" << "with token " << tokens.get(tokens.index())->getText() << std::endl ;
#endif
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
#ifdef DEBUG
                std::cout << "Exiting Global()" << "with token " << tokens.get(tokens.index())->getText() << std::endl ;
#endif
                return result;
            }
            reportError(*thirdToken);
        }
        reportError(*nextToken);
    }
    reportError(*token);
}


UnitNode* Unit(antlr4::CommonTokenStream &tokens) {
#ifdef DEBUG
    std::cout << "Entering Unit()" << " with token " << tokens.get(tokens.index())->getText() << std::endl ;
#endif
    antlr4::Token *token = tokens.get(tokens.index());
    bool minus = false;

    if (token->getType() == MITScript::MINUS) {
        tokens.consume();
        minus = true;
    }
    if (token->getType() == MITScript::LPAREN) {
        tokens.consume();
        auto boolean = Boolean(tokens);
        antlr4::Token *token = tokens.get(tokens.index());
        if (token->getType() == MITScript::RPAREN){
            tokensb.consume();
            #ifdef DEBUG
                std::cout << "Exiting Unit()" << " with token " << tokens.get(tokens.index())->getText() << std::endl ;
            #endif
            return new UnitNode(minus, nullptr, nullptr, nullptr, boolean);
        }
        reportError(*token);
    }
    if (token->getType() == MITScript::NAME) {
        int cur_token_index = tokens.index();
        auto lhs = LHS(tokens);
        token = tokens.get(tokens.index());
        if (token->getType() == MITScript::LPAREN) {
            tokens.seek(cur_token_index);
            #ifdef DEBUG
                std::cout << "Exiting Unit()" << " with token " << tokens.get(tokens.index())->getText() << std::endl ;
            #endif
            return new UnitNode(minus, nullptr, nullptr, Call(tokens), nullptr);
        }
        #ifdef DEBUG
            std::cout << "Exiting Unit()" << " with token " << tokens.get(tokens.index())->getText() << std::endl ;
        #endif
        return new UnitNode(minus, lhs, nullptr, nullptr, nullptr);
    }




    #ifdef DEBUG
        std::cout << "Exiting Unit()" << " with token " << tokens.get(tokens.index())->getText() << std::endl ;
    #endif
    UnitNode* result = new UnitNode(minus, nullptr, Constant(tokens), nullptr, nullptr);
    return result;
}