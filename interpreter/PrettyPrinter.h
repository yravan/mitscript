#pragma once
/*
Written by me
*/
#include <iostream>
#include <string>

#include "Visitor.h"

// This is where you get to define your pretty printer class, which should be
// a subtype of visitor.

namespace AST{

  class PrettyPrinter : public Visitor {
      int indent_level = 0;
      public:
          void indent(){
              for (int i = 0; i < indent_level; i ++){
                  std::cout << "\t";
              }
          }
          void visit(const AST::Expression& expr) override {
              DEBUG_PRINT("Expression(\n");
              std::cout << expr.name;
              DEBUG_PRINT(")");
          }

          void visit(const AST::Statement& expr) override {
              DEBUG_PRINT("Statement()");
          }

          void visit(const AST::Block& node) override {
              DEBUG_PRINT("Block(\n");
              std::cout << "{" << std::endl;
              indent_level += 1;
              for (AST::Statement * statement: node.statements){
                  statement->accept(*this);
                  std::cout << std::endl;
              }
              indent_level -= 1;
              indent();
              std::cout << "}";
              DEBUG_PRINT(")");
          }

          void visit(const AST::Global& node) override {
              DEBUG_PRINT("Global(\n");
              indent();
              std::cout << "global" << " " << node.name << ";";
              DEBUG_PRINT(")");
          }

          void visit(const AST::Assignment& node) override {
              DEBUG_PRINT("Assignment(\n");
              indent();
              node.lhs->accept(*this);  // Use accept to dispatch the correct visit method
              std::cout << " = ";
              node.expr->accept(*this);
              std::cout << ";";
              DEBUG_PRINT(")");
          }

          void visit(const AST::CallStatement& node) override {
              DEBUG_PRINT("CallStatement(\n");
              indent();
              node.callExpr->accept(*this);  // Use accept for the call expression
              std::cout << ';' << std::endl;
              DEBUG_PRINT(")");
          }

          void visit(const AST::IfStatement& node) override {
              DEBUG_PRINT("IfStatement(\n");
              indent();
              std::cout << "if (";
              node.condition->accept(*this);  // Use accept for the condition
              std::cout << ") ";
              node.thenPart->accept(*this);  // Use accept for the then part
              if (node.elsePart != nullptr) {
                  std::cout << " else ";
                  node.elsePart->accept(*this);  // Use accept for the else part, if it exists
              }
              DEBUG_PRINT(")");
          }

          void visit(const AST::WhileLoop& node) override {
              DEBUG_PRINT("WhileLoop(\n");
              indent();
              std::cout << "while (";
              node.condition->accept(*this);  // Use accept for the condition
              std::cout << ") ";
              node.body->accept(*this);  // Use accept for the body
              DEBUG_PRINT(")");
          }

          void visit(const AST::Return& node) override {
              DEBUG_PRINT("Return(\n");
              indent();
              std::cout << "return ";
              node.expr->accept(*this);  // Use accept for the expression
              std::cout << ";";
              DEBUG_PRINT(")");
          }

          void visit(const AST::FunctionDeclaration& node) override {
              DEBUG_PRINT("FunctionDeclaration(\n");
              std::cout << "function(";
              for (size_t i = 0; i < node.arguments.size(); i++) {
                  if (i != 0) std::cout << ", ";
                  std::cout << node.arguments[i];  // Print argument names directly
              }
              std::cout << ")";
              node.body->accept(*this);  // Use accept for the function body
              DEBUG_PRINT(")");
          }

          void visit(const AST::BinaryExpression& node) override {
              DEBUG_PRINT("BinaryExpression(\n");
              std::cout << "("; 
              node.left->accept(*this);  // Use accept for the left operand
              std::cout << " " << this->operator_to_string(node.op) << " ";
              node.right->accept(*this);  // Use accept for the right operand
              std::cout << ")";
              DEBUG_PRINT(")");
          }

          void visit(const AST::UnaryExpression& node) override {
              DEBUG_PRINT("UnaryExpression(\n");
              std::cout << "(";
              std::cout << this->operator_to_string(node.op) << " ";  // Unary operator printed first
              node.operand->accept(*this);  // Use accept for the operand
              std::cout << ")";
              DEBUG_PRINT(")");
          }

          void visit(const AST::FieldDereference& node) override {
              DEBUG_PRINT("FieldDereference(\n");
              node.baseExpr->accept(*this);  // Use accept for the base expression
              std::cout << "." << node.field;  // Print the field name
              DEBUG_PRINT(")");
          }

          void visit(const AST::IndexExpression& node) override {
              DEBUG_PRINT("IndexExpression(\n");
              node.baseExpr->accept(*this);  // Use accept for the base expression
              std::cout << "[";
              node.index->accept(*this);  // Use accept for the index expression
              std::cout << "]";
              DEBUG_PRINT(")");
          }

          void visit(const AST::Call& node) override {
              DEBUG_PRINT("Call(\n");
              node.targetExpr->accept(*this);  // Use accept for the target expression
              std::cout << "(";
              for (size_t i = 0; i < node.arguments.size(); i++) {
                  if (i != 0) std::cout << ", ";
                  node.arguments[i]->accept(*this);  // Use accept for each argument
              }
              std::cout << ")";
              DEBUG_PRINT(")");
          }

          void visit(const AST::Record& node) override {
              DEBUG_PRINT("Record(\n");
              std::cout << "{" << std::endl;
              indent_level += 1;
              for (const auto& field : node.fields) {
                  indent();
                  std::cout << field.first << ": ";  // Print the field name
                  field.second->accept(*this);  // Use accept for the field value
                  std::cout << ";" << std::endl;
              }
              indent_level -= 1;
              indent();
              std::cout << "}";
              DEBUG_PRINT(")");
          }

          void visit(const AST::IntegerConstant& node) override {
              DEBUG_PRINT("IntegerConstant(\n");
              std::cout << node.value;
              DEBUG_PRINT(")");
          }

          void visit(const AST::StringConstant& node) override {
              DEBUG_PRINT("StringConstant(\n");
              std::cout << "\"" << node.value << "\"";
              DEBUG_PRINT(")");
          }

          void visit(const AST::NoneConstant& node) override {
              DEBUG_PRINT("NoneConstant(\n");
              std::cout << "none";
              DEBUG_PRINT(")");
          }

          void visit(const AST::BooleanConstant& node) override {
              DEBUG_PRINT("BooleanConstant(\n");
              std::cout << (node.value ? "true" : "false");
              DEBUG_PRINT(")");
          }

          void visit(const AST::Program& node) override {
              DEBUG_PRINT("Program(\n");
              if (node.mainBlock != nullptr){
                  node.mainBlock->accept(*this);  // Use accept for the main block of the program
              }
              std :: cout << std::endl;
              DEBUG_PRINT(")");
          }


  };
}