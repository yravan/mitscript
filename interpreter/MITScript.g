lexer grammar MITScript;

// Lexer Rules

INT : [0-9]+ ;

MUL : '*' ;

DIV : '/' ;

COMMENT: '//'~( '\r' | '\n' )* -> skip ;

WHITESPACE : ( '\t' | ' ' | '\r' | '\n'| '\u000C' )+ -> skip ;

// The rest of your lexical rules go here

// Keywords
GLOBAL   : 'global' ;
IF       : 'if' ;
ELSE     : 'else' ;
WHILE    : 'while' ;
RETURN   : 'return' ;
FUN      : 'fun' ;

// Operators
ASSIGN   : '=' ;
PLUS     : '+' ;
MINUS    : '-' ;
// MUL   : '*' ;
// DIV   : '/' ;
LT       : '<' ;
GT       : '>' ;
LE       : '<=' ;
GE       : '>=' ;
EQ       : '==' ;
AND      : '&' ;
OR       : '|' ;
NOT      : '!' ;
DOT      : '.' ;
LBRACE   : '{' ;
RBRACE   : '}' ;
LPAREN   : '(' ;
RPAREN   : ')' ;
LBRACKET : '[' ;
RBRACKET : ']' ;
COLON    : ':' ;
SEMICOLON: ';' ;
COMMA    : ',' ;

// Constants
STRING : '"' (ESC | '\\' ~[\\n"t] . { throw std::runtime_error("Invalid escape sequence detected"); }  | ~["\\])* '"' ;
fragment ESC        : '\\' [\\n"t] ;

// Boolean literals
TRUE     : 'true' ;
FALSE    : 'false' ;
BOOLEAN  : TRUE | FALSE ;
NONE     : 'None' ;

// Identifiers
NAME     : [a-zA-Z_][a-zA-Z_0-9]* ;

// Whitespace and comments (skipped)
// WS       : [ \t\r\n]+ -> skip ;
// COMMENT: '//' ~[\r\n]* -> skip ;

ERROR : . ; // Must keep as last rule in grammar!
