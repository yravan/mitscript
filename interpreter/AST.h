#pragma once
/*
Written by me
*/
#include <iostream>
#include <map>
#include <memory>
#include <vector>
#include <unordered_map>

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

    enum Operator{
        PLUS, SUB, MUL, DIV,
        AND, OR, NEG,
        GT, LT, GEQ, LEQ, EQ
    };

    class AST_node {
    public:
        virtual void accept(Visitor &v) const = 0;
        virtual bool equals(AST_node&) = 0;
    };

    // Program class representing the main program
    class Program : public AST_node {
    public:
        class Block* mainBlock; // Forward declaration for Block
        Program(Block* mainBlock_) : mainBlock(mainBlock_) {}
        void accept(Visitor &v) const override;
        virtual bool equals(AST_node&) override;
    };

    // Base class for expressions
    class Expression : public AST_node {
    public:
        std::string name;
        Expression(const std::string& name_) : name(name_) {}
        Expression() : name("") {}
        void accept(Visitor &v) const override;
        virtual bool equals(AST_node&) override;
    };

    // Base class for statements
    class Statement : public AST_node {
        public:
        void accept(Visitor &v) const override;
        virtual bool equals(AST_node&) override;
    };

    // Block class representing a list of statements
    class Block : public Statement {
    public:
        std::vector<Statement*> statements;
        Block(const std::vector<Statement*>& stmts) : statements(stmts) {}
        void accept(Visitor &v) const override;
        virtual bool equals(AST_node&) override;
    };

    // Global class representing a global variable
    class Global : public Statement {
    public:
        std::string name;
        Global(const std::string& name_) : name(name_) {}
        void accept(Visitor &v) const override;
        virtual bool equals(AST_node&) override;
    };

    // Assignment class representing an assignment statement
    class Assignment : public Statement {
    public:
        Expression* lhs;
        Expression* expr;
        Assignment(Expression* lhs_, Expression* expr_) : lhs(lhs_), expr(expr_) {}
        void accept(Visitor &v) const override;
        virtual bool equals(AST_node&) override;
    };

    // CallStatement class representing a call statement
    class CallStatement : public Statement {
    public:
        class Call* callExpr; // Forward declaration for Call
        CallStatement(Call* callExpr_) : callExpr(callExpr_) {}
        void accept(Visitor &v) const override;
        virtual bool equals(AST_node&) override;
    };

    // IfStatement class representing an if statement
    class IfStatement : public Statement {
    public:
        Expression* condition;
        Block* thenPart;
        Block* elsePart;
        IfStatement(Expression* condition_, Block* thenPart_, Block* elsePart_)
            : condition(condition_), thenPart(thenPart_), elsePart(elsePart_) {}
        void accept(Visitor &v) const override;
        virtual bool equals(AST_node&) override;
    };

    // WhileLoop class representing a while loop
    class WhileLoop : public Statement {
    public:
        Expression* condition;
        Block* body;
        WhileLoop(Expression* condition_, Block* body_)
            : condition(condition_), body(body_) {}
        void accept(Visitor &v) const override;
        virtual bool equals(AST_node&) override;
    };

    // Return class representing a return statement
    class Return : public Statement {
    public:
        Expression* expr;
        Return(Expression* expr_) : expr(expr_) {}
        void accept(Visitor &v) const override;
        virtual bool equals(AST_node&) override;
    };

    // FunctionDeclaration class representing a function declaration
    class FunctionDeclaration : public Expression {
    public:
        std::vector<std::string> arguments;
        Block* body;
        FunctionDeclaration(const std::vector<std::string>& args, Block* body_)
            : arguments(args), body(body_) {}
        void accept(Visitor &v) const override;
        virtual bool equals(AST_node&) override;
    };

    // BinaryExpression class representing a binary expression
    class BinaryExpression : public Expression {
    public:
        Expression* left;
        Operator op;
        Expression* right;
        BinaryExpression(Expression* left_, const Operator& op_, Expression* right_)
            : left(left_), op(op_), right(right_) {}
        void accept(Visitor &v) const override;
        virtual bool equals(AST_node&) override;
    };

    // UnaryExpression class representing a unary expression
    class UnaryExpression : public Expression {
    public:
        // the operator is always preceding (! or -)
        Expression* operand;
        Operator op;
        UnaryExpression(Expression* operand_, const Operator& op_)
            : operand(operand_), op(op_) {}
        void accept(Visitor &v) const override;
        virtual bool equals(AST_node&) override;
    };

    // FieldDereference class representing a field dereference
    class FieldDereference : public Expression {
    public:
        Expression* baseExpr;
        std::string field;
        FieldDereference(Expression* baseExpr_, const std::string& field_)
            : baseExpr(baseExpr_), field(field_) {}
        void accept(Visitor &v) const override;
        virtual bool equals(AST_node&) override;
    };

    // IndexExpression class representing an index expression
    class IndexExpression : public Expression {
    public:
        Expression* baseExpr;
        Expression* index;
        IndexExpression(Expression* baseExpr_, Expression* index_)
            : baseExpr(baseExpr_), index(index_) {}
        void accept(Visitor &v) const override;
        virtual bool equals(AST_node&) override;
    };

    // Call class representing a function call
    class Call : public Expression {
    public:
        Expression* targetExpr;
        std::vector<Expression*> arguments;
        Call(Expression* targetExpr_, const std::vector<Expression*>& args)
            : targetExpr(targetExpr_), arguments(args) {}
        void accept(Visitor &v) const override;
        virtual bool equals(AST_node&) override;
    };

    // Record class representing a record structure
    class Record : public Expression {
    public:
        std::vector<std::pair<std::string, Expression*>> fields;
        Record(const std::vector<std::pair<std::string, Expression*>> & fields_) : fields(fields_) {}
        void accept(Visitor &v) const override;
        virtual bool equals(AST_node&) override;
    };

    // IntegerConstant class representing an integer constant
    class IntegerConstant : public Expression {
    public:
        int value;
        IntegerConstant(int value_) : value(value_) {}
        void accept(Visitor &v) const override;
        virtual bool equals(AST_node&) override;
    };

    // StringConstant class representing a string constant
    class StringConstant : public Expression {
    public:
        std::string value;
        StringConstant(const std::string& value_) : value(value_) {}
        void accept(Visitor &v) const override;
        virtual bool equals(AST_node&) override;
    };

    // NoneConstant class representing a none constant
    class NoneConstant : public Expression {
    public:
        NoneConstant() {}
        void accept(Visitor &v) const override;
        virtual bool equals(AST_node&) override;
    };

    // BooleanConstant class representing a boolean constant
    class BooleanConstant : public Expression {
    public:
        bool value;
        BooleanConstant(bool value_) : value(value_) {}
        void accept(Visitor &v) const override;
        virtual bool equals(AST_node&) override;
    };
    
}
