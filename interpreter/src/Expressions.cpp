#include "nonterminals.h"

RecordPrimeNode* RecordPrime(antlr4::CommonTokenStream &tokens){
  antlr4::Token *token = tokens.get(tokens.index());
  if (token->getType() == MITScript::NAME){
    tokens.consume();
    antlr4::Token *token_2 = tokens.get(tokens.index());
    if (token_2->getType() == MITScript::COLON){
        tokens.consume();
        expr = Expression(tokens);

        antlr4::Token *token_3 = tokens.get(tokens.index());
        if (token_3->getType() == MITScript::SEMICOLON){
            tokens.consume();
            return new RecordPrimeNode(token, expr, RecordPrime(tokens));
        }
    }
  }
  return NULL;
}
RecordNode* Record(antlr4::CommonTokenStream &tokens){
  antlr4::Token *token = tokens.get(tokens.index());
  if (token->getType() == MITScript::LBRACE){
    tokens.consume();
    auto inside =  RecordPrime(tokens);
    antlr4::Token *next_token = tokens.get(tokens.index());
    if (next_token->getType() == MITScript::RBRACE){
        tokens.consume();
        return new RecordNode(inside);
    }
    reportError(*next_token);
  }
  reportError(*token);
}


LHSPrimeNode* LHSPrime(antlr4::CommonTokenStream &tokens){
  antlr4::Token *token = tokens.get(tokens.index());
  if (token->getType() == MITScript::DOT){
    tokens.consume();
    antlr4::Token *token_2 = tokens.get(tokens.index());
    if (token_2->getType() == MITScript::NAME){
        tokens.consume();
        return new LHSPrimeNode(token_2, NULL, LHSPrime(tokens));
    }
    reportError(*token_2);
  }
  if (token->getType() == MITScript::LBRACKET){
    tokens.consume();
    expr = Expression(tokens);
    antlr4::Token *token_2 = tokens.get(tokens.index());
    if (token_2->getType() == MITScript::RBRACKET){
        tokens.consume();
        return new LHSPrimeNode(NULL, expr, LHSPrime(tokens));
    }
    reportError(*token_2);
  }
  return NULL;
}
LHSNode* LHS(antlr4::CommonTokenStream &tokens){
  antlr4::Token *token = tokens.get(tokens.index());
  if (token->getType() == MITScript::NAME){
    tokens.consume();
    return new LHSNode(token, LHSPrime(tokens));
  }
  reportError(*token);
}



GlobalNode* Global(antlr4::CommonTokenStream &tokens){
  antlr4::Token *token = tokens.get(tokens.index());
  if (token->getType() == MITScript::GLOBAL){
    tokens.consume();
    nextToken = tokens.get(tokens.index());
    if (nextToken->getType() == MITScript::NAME){
        tokens.consume();
        thirdToken = tokens.get(tokens.index());
        if (thirdToken->getType() == MITScript::SEMICOLON){
            return new GlobalNode(nextToken);
        }
        reportError(*thirdToken);
    }
    reportError(*nextToken);
  }
  reportError(*token);

}

CallStatementNode* CallStatement(antlr4::CommonTokenStream &tokens){
    call = Call(tokens);
    antlr4::Token *token = tokens.get(tokens.index());
    if (token->getType() == MITScript::SEMICOLON){
        return new CallStatementNode(call);
    }
    reportError(*token);

}

ExpressionNode* Expression(antlr4::CommonTokenStream &tokens); // NOT fully implemented
    antlr4::Token *token = tokens.get(tokens.index());
    
    switch (token ->getType()){
        // case MITScript::FUN:
        //     return new ExpressionNode(Function(tokens), NULL, NULL); 
        //     break;
        case MITScript::LBRACE:
            return new ExpressionNode(NULL, NULL, Record(tokens));
            break;
        default:
            return new ExpressionNode(NULL, Boolean(tokens), NULL);
    }
    assert(0);




