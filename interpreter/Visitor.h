#pragma once

#include <memory>
#include "AST.h"

// You will need to a virtual visitor class with a
// visit method for each different type of expression and statement
// as defined in AST.h
using namespace AST;
class Visitor {
 public:
  // For each AST node, you need a virtual method of the form
  // virtual void visit(EXPRESSION_TYPE& exp)=0;
  virtual void visit(AST_node& node) = 0;
  virtual void visit(Block& node) = 0;
  virtual void visit(Global& node) = 0;
  virtual void visit(Assignment& node) = 0;
  virtual void visit(CallStatement& node) = 0;
  virtual void visit(IfStatement& node) = 0;
  virtual void visit(WhileLoop& node) = 0;
  virtual void visit(Return& node) = 0;
  virtual void visit(FunctionDeclaration& node) = 0;
  virtual void visit(BinaryExpression& node) = 0;
  virtual void visit(UnaryExpression& node) = 0;
  virtual void visit(FieldDereference& node) = 0;
  virtual void visit(IndexExpression& node) = 0;
  virtual void visit(Call& node) = 0;
  virtual void visit(Record& node) = 0;
  virtual void visit(IntegerConstant& node) = 0;
  virtual void visit(StringConstant& node) = 0;
  virtual void visit(NoneConstant& node) = 0;
  virtual void visit(BooleanConstant& node) = 0;
  virtual void visit(Program& node) = 0;
};
