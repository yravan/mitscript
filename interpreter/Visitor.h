#pragma once
/*
Written by me
*/
#include "AST.h"

// You will need to a virtual visitor class with a
// visit method for each different type of expression and statement
// as defined in AST.h

class Visitor {
 public:

    std::string operator_to_string(AST::Operator op){
        switch (op) {
            case AST::Operator::PLUS:
                return "+";
            case AST::Operator::SUB:
                return "-";
            case AST::Operator::MUL:
                return "*";
            case AST::Operator::DIV:
                return "/";
            case AST::Operator::OR:
                return "|";
            case AST::Operator::AND:
                return "&";
            case AST::Operator::NEG:
                return "!";
            case AST::Operator::LT:
                return "<";
            case AST::Operator::GT:
                return ">";
            case AST::Operator::LEQ:
                return "<=";
            case AST::Operator::GEQ:
                return ">=";
            case AST::Operator::EQ:
                return "==";
            default:
                throw std::runtime_error("Unsupported operator");
        }
    }
  // For each AST node, you need a virtual method of the form
  // virtual void visit(const AST::EXPRESSION_TYPE& exp)=0;
  virtual void visit(const AST::Block& node) = 0;
  virtual void visit(const AST::Global& node) = 0;
  virtual void visit(const AST::Expression& node) = 0;
  virtual void visit(const AST::Statement& node) = 0;
  virtual void visit(const AST::Assignment& node) = 0;
  virtual void visit(const AST::CallStatement& node) = 0;
  virtual void visit(const AST::IfStatement& node) = 0;
  virtual void visit(const AST::WhileLoop& node) = 0;
  virtual void visit(const AST::Return& node) = 0;
  virtual void visit(const AST::FunctionDeclaration& node) = 0;
  virtual void visit(const AST::BinaryExpression& node) = 0;
  virtual void visit(const AST::UnaryExpression& node) = 0;
  virtual void visit(const AST::FieldDereference& node) = 0;
  virtual void visit(const AST::IndexExpression& node) = 0;
  virtual void visit(const AST::Call& node) = 0;
  virtual void visit(const AST::Record& node) = 0;
  virtual void visit(const AST::IntegerConstant& node) = 0;
  virtual void visit(const AST::StringConstant& node) = 0;
  virtual void visit(const AST::NoneConstant& node) = 0;
  virtual void visit(const AST::BooleanConstant& node) = 0;
  virtual void visit(const AST::Program& node) = 0;
};
