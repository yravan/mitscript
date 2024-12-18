#pragma once
#include "AST.h"
#include "Visitor.h"
#include "types.h"
#include "symbol_table.h"
#include <unordered_set>

using BasicBlock = std::vector<Instruction>;

class Compiler: Visitor {
  Function* enclosing_function_;
  Function* global_function_;
  SymbolTable* symbol_table_;
  SymbolTable* global_symbol_table_;
  // std::unordered_set<Constant*> current_constants;
  BasicBlock current_block_;
  bool lhs_;

public:
  void visit(const AST::Block& node) override;
  void visit(const AST::Global& node) override;
  void visit(const AST::Expression& node) override;
  void visit(const AST::Statement& node) override;
  void visit(const AST::Assignment& node) override;
  void visit(const AST::CallStatement& node) override;
  void visit(const AST::IfStatement& node) override;
  void visit(const AST::WhileLoop& node) override;
  void visit(const AST::Return& node) override;
  void visit(const AST::FunctionDeclaration& node) override;
  void visit(const AST::BinaryExpression& node) override;
  void visit(const AST::UnaryExpression& node) override;
  void visit(const AST::FieldDereference& node) override;
  void visit(const AST::IndexExpression& node) override;
  void visit(const AST::Call& node) override;
  void visit(const AST::Record& node) override;
  void visit(const AST::IntegerConstant& node) override;
  void visit(const AST::StringConstant& node) override;
  void visit(const AST::NoneConstant& node) override;
  void visit(const AST::BooleanConstant& node) override;
  void visit(const AST::Program& node) override;

  void globals(const AST::Statement& s);
  void assigns(const AST::Statement& s);
  bool findReturn(const AST::Statement& s);

  Function* compile(const AST::Program& program);

  int alloc_constant(Constant& constant);
  int alloc_name(const std::string& name);

};

