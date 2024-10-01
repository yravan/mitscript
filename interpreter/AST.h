#pragma once
/*
Written by me
*/
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
namespace AST {

    class AST_node {
    public:
        virtual void accept(Visitor &v) = 0;
    };

    // Program class representing the main program
    class Program : public AST_node {
    public:
        class Block* mainBlock; // Forward declaration for Block
        Program(Block* mainBlock_) : mainBlock(mainBlock_) {}
        void accept(Visitor &v) override;
    };

    // Base class for expressions
    class Expression : public AST_node {
    public:
        std::string name;
        Expression(const std::string& name_) : name(name_) {}
        Expression() : name("") {}
        void accept(Visitor &v) override;
    };

    // Base class for statements
    class Statement : public AST_node {
        public:
        void accept(Visitor &v)=0;
    };

    // Block class representing a list of statements
    class Block : public Statement {
    public:
        std::vector<Statement*> statements;
        Block(const std::vector<Statement*>& stmts) : statements(stmts) {}
        void accept(Visitor &v) override;
    };

    // Global class representing a global variable
    class Global : public Statement {
    public:
        std::string name;
        Global(const std::string& name_) : name(name_) {}
        void accept(Visitor &v) override;
    };

    // Assignment class representing an assignment statement
    class Assignment : public Statement {
    public:
        Expression* lhs;
        Expression* expr;
        Assignment(Expression* lhs_, Expression* expr_) : lhs(lhs_), expr(expr_) {}
        void accept(Visitor &v) override;
    };

    // CallStatement class representing a call statement
    class CallStatement : public Statement {
    public:
        class Call* callExpr; // Forward declaration for Call
        CallStatement(Call* callExpr_) : callExpr(callExpr_) {}
        void accept(Visitor &v) override;
    };

    // IfStatement class representing an if statement
    class IfStatement : public Statement {
    public:
        Expression* condition;
        Block* thenPart;
        Block* elsePart;
        IfStatement(Expression* condition_, Block* thenPart_, Block* elsePart_)
            : condition(condition_), thenPart(thenPart_), elsePart(elsePart_) {}
        void accept(Visitor &v) override;
    };

    // WhileLoop class representing a while loop
    class WhileLoop : public Statement {
    public:
        Expression* condition;
        Block* body;
        WhileLoop(Expression* condition_, Block* body_)
            : condition(condition_), body(body_) {}
        void accept(Visitor &v) override;
    };

    // Return class representing a return statement
    class Return : public Statement {
    public:
        Expression* expr;
        Return(Expression* expr_) : expr(expr_) {}
        void accept(Visitor &v) override;
    };

    // FunctionDeclaration class representing a function declaration
    class FunctionDeclaration : public Expression {
    public:
        std::vector<std::string> arguments;
        Block* body;
        FunctionDeclaration(const std::vector<std::string>& args, Block* body_)
            : arguments(args), body(body_) {}
        void accept(Visitor &v) override;
    };

    // BinaryExpression class representing a binary expression
    class BinaryExpression : public Expression {
    public:
        Expression* left;
        std::string op;
        Expression* right;
        BinaryExpression(Expression* left_, const std::string& op_, Expression* right_)
            : left(left_), op(op_), right(right_) {}
        void accept(Visitor &v) override;
    };

    // UnaryExpression class representing a unary expression
    class UnaryExpression : public Expression {
    public:
        Expression* operand;
        std::string op;
        bool preceding;
        UnaryExpression(Expression* operand_, const std::string& op_, bool preceding_)
            : operand(operand_), op(op_), preceding(preceding_) {}
        void accept(Visitor &v) override;
    };

    // FieldDereference class representing a field dereference
    class FieldDereference : public Expression {
    public:
        Expression* baseExpr;
        std::string field;
        FieldDereference(Expression* baseExpr_, const std::string& field_)
            : baseExpr(baseExpr_), field(field_) {}
        void accept(Visitor &v) override;
    };

    // IndexExpression class representing an index expression
    class IndexExpression : public Expression {
    public:
        Expression* baseExpr;
        Expression* index;
        IndexExpression(Expression* baseExpr_, Expression* index_)
            : baseExpr(baseExpr_), index(index_) {}
        void accept(Visitor &v) override;
    };

    // Call class representing a function call
    class Call : public Expression {
    public:
        Expression* targetExpr;
        std::vector<Expression*> arguments;
        Call(Expression* targetExpr_, const std::vector<Expression*>& args)
            : targetExpr(targetExpr_), arguments(args) {}
        void accept(Visitor &v) override;
    };

    // Record class representing a record structure
    class Record : public Expression {
    public:
        std::map<std::string, Expression*> fields;
        Record(const std::map<std::string, Expression*>& fields_) : fields(fields_) {}
        void accept(Visitor &v) override;
    };

    // IntegerConstant class representing an integer constant
    class IntegerConstant : public Expression {
    public:
        int value;
        IntegerConstant(int value_) : value(value_) {}
        void accept(Visitor &v) override;
    };

    // StringConstant class representing a string constant
    class StringConstant : public Expression {
    public:
        std::string value;
        StringConstant(const std::string& value_) : value(value_) {}
        void accept(Visitor &v) override;
    };

    // NoneConstant class representing a none constant
    class NoneConstant : public Expression {
    public:
        NoneConstant() {}
        void accept(Visitor &v) override;
    };

    // BooleanConstant class representing a boolean constant
    class BooleanConstant : public Expression {
    public:
        bool value;
        BooleanConstant(bool value_) : value(value_) {}
        void accept(Visitor &v) override;
    };
}
