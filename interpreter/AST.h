#pragma once

#include <iostream>
#include <map>
#include <memory>
#include <vector>

class Visitor;

#define Assert(cond, msg)               \
  if (!(cond)) {                        \
    std::cerr << msg << std::endl;      \
    throw SystemException("Bad stuff"); \
  }

class SystemException {
  std::string msg_;

 public:
  SystemException(const std::string &msg) : msg_(msg) {}
};

namespace AST{
class AST_node {
public:
    void accept(Visitor &v);
};

// Program class representing the main program
class Program : public AST_node {
public:
    class Block* mainBlock; // Forward declaration for Block

    Program(Block* mainBlock_);
};

// Base class for expressions
class Expression : public AST_node {
public:
    std::string name;

    Expression(const std::string& name_);
    Expression();
};

// Base class for statements
class Statement : public AST_node {};

// Block class representing a list of statements
class Block : public Statement {
public:
    std::vector<Statement*> statements;

    Block(const std::vector<Statement*>& stmts);
};

// Global class representing a global variable
class Global : public Statement {
public:
    std::string name;

    Global(const std::string& name_);
};

// Assignment class representing an assignment statement
class Assignment : public Statement {
public:
    Expression* lhs;
    Expression* expr;

    Assignment(Expression* lhs_, Expression* expr_);
};

// CallStatement class representing a call statement
class CallStatement : public Statement {
public:
    class Call* callExpr; // Forward declaration for Call

    CallStatement(Call* callExpr_);
};

// IfStatement class representing an if statement
class IfStatement : public Statement {
public:
    Expression* condition;
    Block* thenPart;
    Block* elsePart;

    IfStatement(Expression* condition_, Block* thenPart_, Block* elsePart_);
};

// WhileLoop class representing a while loop
class WhileLoop : public Statement {
public:
    Expression* condition;
    Block* body;

    WhileLoop(Expression* condition_, Block* body_);
};

// Return class representing a return statement
class Return : public Statement {
public:
    Expression* expr;

    Return(Expression* expr_);
};

// FunctionDeclaration class representing a function declaration
class FunctionDeclaration : public Statement {
public:
    std::vector<std::string> arguments;
    Block* body;

    FunctionDeclaration(const std::vector<std::string>& args, Block* body_);
};

// BinaryExpression class representing a binary expression
class BinaryExpression : public Expression {
public:
    Expression* left;
    std::string op;
    Expression* right;

    BinaryExpression(Expression* left_, const std::string& op_, Expression* right_);
};

// UnaryExpression class representing a unary expression
class UnaryExpression : public Expression {
public:
    Expression* operand;
    std::string op;
    bool preceding;

    UnaryExpression(Expression* operand_, const std::string& op_, bool preceding_);
};

// FieldDereference class representing a field dereference
class FieldDereference : public Expression {
public:
    Expression* baseExpr;
    std::string field;

    FieldDereference(Expression* baseExpr_, const std::string& field_);
};

// IndexExpression class representing an index expression
class IndexExpression : public Expression {
public:
    Expression* baseExpr;
    Expression* index;

    IndexExpression(Expression* baseExpr_, Expression* index_);
};

// Call class representing a function call
class Call : public Expression {
public:
    Expression* targetExpr;
    std::vector<Expression*> arguments;

    Call(Expression* targetExpr_, const std::vector<Expression*>& args);
};

// Record class representing a record structure
class Record : public Expression {
public:
    std::map<std::string, Expression*> fields;

    Record(const std::map<std::string, Expression*>& fields_);
};

// IntegerConstant class representing an integer constant
class IntegerConstant : public Expression {
public:
    int value;

    IntegerConstant(int value_);
};

// StringConstant class representing a string constant
class StringConstant : public Expression {
public:
    std::string value;

    StringConstant(const std::string& value_);
};

// NoneConstant class representing a none constant
class NoneConstant : public Expression {
public:
    NoneConstant();
};

// BooleanConstant class representing a boolean constant
class BooleanConstant : public Expression {
public:
    bool value;

    BooleanConstant(bool value_);
};
}
