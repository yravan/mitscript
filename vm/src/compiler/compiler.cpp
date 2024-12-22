#include "compiler.h"
#include <assert.h>

Function* Compiler::compile(const AST::Program& program) {
  program.accept(*this);
  return enclosing_function_;
}


void Compiler::visit(const AST::Block& node) {
  BasicBlock block = BasicBlock();
  for (const auto& statement : node.statements){
    statement->accept(*this);
    block.insert(block.end(), current_block_.begin(), current_block_.end());
  }
  current_block_ = block;
}
void Compiler::visit(const AST::Global& node) {
  current_block_ = BasicBlock();
  alloc_name(node.name);
  symbol_table_->insert(node.name, SymbolTable::SymbolType::GLOBAL);
  global_function_->addName(node.name);
  global_symbol_table_->insert(node.name, SymbolTable::SymbolType::GLOBAL);
}
void Compiler::visit(const AST::Expression& node) {
  current_block_ = BasicBlock();
  if (lhs_){
    if (symbol_table_->lookup(node.name) == SymbolTable::SymbolType::LOCAL){
      current_block_.push_back(Instruction(Operation::StoreLocal, enclosing_function_->getLocalVar(node.name)));
      return;
    }
    if (symbol_table_->lookup(node.name) == SymbolTable::SymbolType::GLOBAL){
      alloc_name(node.name);current_block_.push_back(Instruction(Operation::StoreGlobal, enclosing_function_->getGlobalVar(node.name)));
      return;
    }
    enclosing_function_->addFreeVar(node.name);
    //set the index later (when all the local_ref_vars are processed)
    current_block_.push_back(Instruction(Operation::PushReference, node.name));
    current_block_.push_back(Instruction(Operation::StoreReference));
    
  } else {
    if (symbol_table_->lookup(node.name) == SymbolTable::SymbolType::LOCAL){
      current_block_.push_back(Instruction(Operation::LoadLocal, enclosing_function_->getLocalVar(node.name)));
      return;
    }
    if (symbol_table_->lookup(node.name) == SymbolTable::SymbolType::GLOBAL){
      alloc_name(node.name);
      current_block_.push_back(Instruction(Operation::LoadGlobal, enclosing_function_->getGlobalVar(node.name)));
      return;
    }
    enclosing_function_->addFreeVar(node.name);
    //set the index later (when all the local_ref_vars are processed)
    current_block_.push_back(Instruction(Operation::PushReference, node.name));
    current_block_.push_back(Instruction(Operation::LoadReference));
  }
}
void Compiler::visit(const AST::Statement& node) {}
void Compiler::visit(const AST::Assignment& node) {
  lhs_ = true;
  node.lhs->accept(*this);
  BasicBlock lhs_block = current_block_;
  lhs_ = false;
  node.expr->accept(*this);
  BasicBlock expr_block = current_block_;

  //insert rhs before lhs
  lhs_block.insert(lhs_block.end() - 1, std::make_move_iterator(expr_block.begin()), std::make_move_iterator(expr_block.end()));
  current_block_ = lhs_block;
}
void Compiler::visit(const AST::CallStatement& node) {
  node.callExpr->accept(*this);
  current_block_.push_back(Instruction(Operation::Pop));
}
void Compiler::visit(const AST::IfStatement& node) {
  node.condition->accept(*this);
  BasicBlock condition_block = current_block_;
  node.thenPart->accept(*this);
  BasicBlock then_block = current_block_;
  if (node.elsePart){
    node.elsePart->accept(*this);
    BasicBlock else_block = current_block_;

    else_block.push_back(Instruction(Operation::Goto, then_block.size() + 1));
    condition_block.push_back(Instruction(Operation::If, else_block.size() + 1));

    condition_block.insert(condition_block.end(), std::make_move_iterator(else_block.begin()), std::make_move_iterator(else_block.end()));
    condition_block.insert(condition_block.end(), std::make_move_iterator(then_block.begin()), std::make_move_iterator(then_block.end()));
  }
  else{
    condition_block.push_back(Instruction(Operation::Not));
    condition_block.push_back(Instruction(Operation::If, then_block.size() + 1));
    condition_block.insert(condition_block.end(), std::make_move_iterator(then_block.begin()), std::make_move_iterator(then_block.end()));
  }
  current_block_ = condition_block;
}
void Compiler::visit(const AST::WhileLoop& node) {
  node.condition->accept(*this);
  BasicBlock condition_block = current_block_;
  node.body->accept(*this);
  BasicBlock body_block = current_block_;
  condition_block.push_back(Instruction(Operation::Not));
  condition_block.push_back(Instruction(Operation::If, body_block.size() + 2));
  body_block.push_back(Instruction(Operation::Goto, -(condition_block.size() + body_block.size())));
  condition_block.insert(condition_block.end(), std::make_move_iterator(body_block.begin()), std::make_move_iterator(body_block.end()));
  current_block_ = condition_block;
}
void Compiler::visit(const AST::Return& node) {
  node.expr->accept(*this);
  current_block_.push_back(Instruction(Operation::Return));
}
void Compiler::visit(const AST::FunctionDeclaration& node) {
  Function* prev_function = enclosing_function_;
  enclosing_function_ = new Function();
  SymbolTable* prev_symbol_table = symbol_table_;
  symbol_table_ = new SymbolTable(symbol_table_);
  prev_function->addFunction(enclosing_function_);
  BasicBlock function_block = BasicBlock();
  function_block.push_back(Instruction(Operation::LoadFunc, prev_function->getFunction(enclosing_function_)));

  for (const auto& arg : node.arguments){
    enclosing_function_->addLocalVar(arg);
    symbol_table_->insert(arg, SymbolTable::SymbolType::LOCAL);
  }
  globals(*node.body);
  assigns(*node.body);
  enclosing_function_->setParameterCount(node.arguments.size());
  node.body->accept(*this);
  if(!findReturn(*node.body)){
    Constant* constant = new Constant::None();
    int index = alloc_constant(*constant);
    current_block_.push_back(Instruction(Operation::LoadConst, index));
    current_block_.push_back(Instruction(Operation::Return));
  }

  enclosing_function_->setInstructionList(current_block_);
  for (Instruction& instr: enclosing_function_->getInstructions()){
    if (instr.operation == Operation::PushReference && instr.name_ != ""){
      instr.setOperand0(enclosing_function_->getFreeVar(instr.name_));
    }
  }
  for (std::string var: enclosing_function_->getFreeVars()){
    if (prev_function->findLocalVar(var)){
      prev_function->addLocalReferenceVar(var);
      function_block.push_back(Instruction(Operation::PushReference, prev_function->getLocalReferenceVar(var)));
    } else {
      prev_function->addFreeVar(var);
      function_block.push_back(Instruction(Operation::PushReference, var));
    }
  }
  function_block.push_back(Instruction(Operation::AllocClosure, enclosing_function_->numFreeVars())); 

  enclosing_function_ = prev_function;
  delete symbol_table_;
  symbol_table_ = prev_symbol_table;
  current_block_ = function_block;
}
void Compiler::visit(const AST::BinaryExpression& node) {
  node.left->accept(*this);
  BasicBlock first_block = current_block_;
  node.right->accept(*this);
  BasicBlock second_block_ = current_block_;
  if (node.op == AST::Operator::LT || node.op == AST::Operator::LEQ){
    std::swap(first_block, second_block_);
  }
  first_block.insert(first_block.end(), std::make_move_iterator(second_block_.begin()), std::make_move_iterator(second_block_.end()));
  current_block_ = first_block;
  switch (node.op){
    case AST::Operator::PLUS:
      current_block_.push_back(Instruction(Operation::Add));
      break;
    case AST::Operator::SUB:
      current_block_.push_back(Instruction(Operation::Sub));
      break;
    case AST::Operator::MUL: 
      current_block_.push_back(Instruction(Operation::Mul));
      break;
    case AST::Operator::DIV:
      current_block_.push_back(Instruction(Operation::Div));
      break;
    case AST::Operator::AND:
      current_block_.push_back(Instruction(Operation::And));
      break;
    case AST::Operator::OR:
      current_block_.push_back(Instruction(Operation::Or));
      break;
    case AST::Operator::NEG:
      assert(false);
      break;
    case AST::Operator::GT:
    case AST::Operator::LT:
      current_block_.push_back(Instruction(Operation::Gt));
      break;
    case AST::Operator::GEQ:
    case AST::Operator::LEQ:
      current_block_.push_back(Instruction(Operation::Geq));
      break;
    case AST::Operator::EQ:
      current_block_.push_back(Instruction(Operation::Eq));
      break;
    default:
      assert(false);
  }
}
void Compiler::visit(const AST::UnaryExpression& node) {
  node.operand->accept(*this);
  switch (node.op){
    case AST::Operator::SUB:
      current_block_.push_back(Instruction(Operation::Neg));
      break;
    case AST::Operator::NEG:
      current_block_.push_back(Instruction(Operation::Not));
      break;
    default:
      assert(false);
  }
}
void Compiler::visit(const AST::FieldDereference& node) {
  bool assign = lhs_;
  lhs_ = false;
  node.baseExpr->accept(*this);
  if (assign){
    current_block_.push_back(Instruction(Operation::FieldStore, alloc_name(node.field)));
  } else {
    current_block_.push_back(Instruction(Operation::FieldLoad, alloc_name(node.field)));
  }
}
void Compiler::visit(const AST::IndexExpression& node) {
  bool assign = lhs_;
  lhs_ = false;
  node.baseExpr->accept(*this);
  BasicBlock base_block = current_block_;
  node.index->accept(*this);
  BasicBlock index_block = current_block_;
  base_block.insert(base_block.end(), std::make_move_iterator(index_block.begin()), std::make_move_iterator(index_block.end()));
  current_block_ = base_block;
  if (assign){
    current_block_.push_back(Instruction(Operation::IndexStore));
  } else {
    current_block_.push_back(Instruction(Operation::IndexLoad));
  }
}
void Compiler::visit(const AST::Call& node) {
  BasicBlock call_block = BasicBlock();
  node.targetExpr->accept(*this);
  call_block.insert(call_block.end(), current_block_.begin(), current_block_.end());
  for (const auto& arg : node.arguments){
    arg->accept(*this);
    call_block.insert(call_block.end(), current_block_.begin(), current_block_.end());
  }
  call_block.push_back(Instruction(Operation::Call, node.arguments.size()));
  current_block_ = call_block;
}
void Compiler::visit(const AST::Record& node) {
  BasicBlock record_block = BasicBlock();
  record_block.push_back(Instruction(Operation::AllocRecord));
  for (const auto& pair : node.fields){
    record_block.push_back(Instruction(Operation::Dup));
  }
  for (const auto& pair : node.fields){
    pair.second->accept(*this);
    record_block.insert(record_block.end(), current_block_.begin(), current_block_.end());
    record_block.push_back(Instruction(Operation::FieldStore, alloc_name(pair.first)));
  }
  current_block_ = record_block;
}

void Compiler::visit(const AST::IntegerConstant& node) {
  current_block_ = BasicBlock();
  Constant* constant = new Constant::Integer(node.value);
  int index = alloc_constant(*constant);
  current_block_.push_back(Instruction(Operation::LoadConst, index));
}

void Compiler::visit(const AST::StringConstant& node) {
  current_block_ = BasicBlock();
  Constant* constant = new Constant::String(node.value);
  int index = alloc_constant(*constant);
  current_block_.push_back(Instruction(Operation::LoadConst, index));
}

void Compiler::visit(const AST::NoneConstant& node) {
  current_block_ = BasicBlock();
  Constant* constant = new Constant::None();
  int index = alloc_constant(*constant);
  current_block_.push_back(Instruction(Operation::LoadConst, index));
}

void Compiler::visit(const AST::BooleanConstant& node) {
  current_block_ = BasicBlock();
  Constant* constant = new Constant::Boolean(node.value);
  int index = alloc_constant(*constant);
  current_block_.push_back(Instruction(Operation::LoadConst, index));
}

void Compiler::visit(const AST::Program& node) {
  enclosing_function_ = new Function();
  symbol_table_ = new SymbolTable();
  global_symbol_table_ = symbol_table_;
  global_function_ = enclosing_function_;

  // Native Functions
  Function* print_function = new Function();
  print_function->setParameterCount(1);
  print_function->addLocalVar("x");
  Function* input_function = new Function();
  Function* intcast_function = new Function();
  intcast_function->setParameterCount(1);
  intcast_function->addLocalVar("x");
  enclosing_function_->addFunction(print_function);
  enclosing_function_->addFunction(input_function);
  enclosing_function_->addFunction(intcast_function);

  enclosing_function_->addName("print");
  enclosing_function_->addName("input");
  enclosing_function_->addName("intcast");
  symbol_table_->insert("print", SymbolTable::SymbolType::GLOBAL);
  symbol_table_->insert("input", SymbolTable::SymbolType::GLOBAL);
  symbol_table_->insert("intcast", SymbolTable::SymbolType::GLOBAL);

  BasicBlock native_function = BasicBlock();
  native_function.push_back(Instruction(Operation::LoadFunc, 0));
  native_function.push_back(Instruction(Operation::AllocClosure, 0));
  native_function.push_back(Instruction(Operation::StoreGlobal, 0));
  native_function.push_back(Instruction(Operation::LoadFunc, 1));
  native_function.push_back(Instruction(Operation::AllocClosure, 0));
  native_function.push_back(Instruction(Operation::StoreGlobal, 1));
  native_function.push_back(Instruction(Operation::LoadFunc, 2));
  native_function.push_back(Instruction(Operation::AllocClosure, 0));
  native_function.push_back(Instruction(Operation::StoreGlobal, 2));

  globals(*node.mainBlock);
  assigns(*node.mainBlock);
  node.mainBlock->accept(*this);

  native_function.insert(native_function.end(), current_block_.begin(), current_block_.end());

  enclosing_function_->setInstructionList(native_function);
}


void Compiler::globals(const AST::Statement& s) {
  if (typeid(s) == typeid(AST::Assignment) || typeid(s) == typeid(AST::Return)){
    // do nothing
  }
  if (typeid(s) == typeid(AST::IfStatement)){
    AST::IfStatement statement = dynamic_cast<const AST::IfStatement&>(s);
    globals(*statement.thenPart);
    if (statement.elsePart != nullptr){
      globals(*statement.elsePart);
    }
  }
  if (typeid(s) == typeid(AST::WhileLoop)){
    AST::WhileLoop statement = dynamic_cast<const AST::WhileLoop&>(s);
    globals(*statement.body);
  }
  if (typeid(s) == typeid(AST::Block)){
    AST::Block statement = dynamic_cast<const AST::Block&>(s);
    if (statement.statements.size() > 0){
      for (const auto& state: statement.statements){
        globals(*state);
      }
    }
  }
  if (typeid(s) == typeid(AST::Global)){
    s.accept(*this);
  }
}

void Compiler::assigns(const AST::Statement& s) {
  if (typeid(s) == typeid(AST::Assignment)){
    AST::Assignment statement = dynamic_cast<const AST::Assignment&>(s);
    if (typeid(*statement.lhs) == typeid(AST::Expression)){
      //VarAssignment
      if (enclosing_function_ == global_function_ || symbol_table_->lookup_locally(statement.lhs->name) == SymbolTable::SymbolType::GLOBAL){
        alloc_name(statement.lhs->name);
        symbol_table_->insert(statement.lhs->name, SymbolTable::SymbolType::GLOBAL);
      }
      else {
        enclosing_function_->addLocalVar(statement.lhs->name);
        symbol_table_->insert(statement.lhs->name, SymbolTable::SymbolType::LOCAL);
      }
    }
    // do nothing
  }
  if (typeid(s) == typeid(AST::Global) || typeid(s) == typeid(AST::Return)){
    // do nothing
  }
  if (typeid(s) == typeid(AST::IfStatement)){
    AST::IfStatement statement = dynamic_cast<const AST::IfStatement&>(s);
    assigns(*statement.thenPart);
    if (statement.elsePart != nullptr){
      assigns(*statement.elsePart);
    }
  }
  if (typeid(s) == typeid(AST::WhileLoop)){
    AST::WhileLoop statement = dynamic_cast<const AST::WhileLoop&>(s);
    assigns(*statement.body);
  }
  if (typeid(s) == typeid(AST::Block)){
    AST::Block statement = dynamic_cast<const AST::Block&>(s);
    if (statement.statements.size() > 0){
      for (const auto& state: statement.statements){
        assigns(*state);
      }
    }
  }
}

bool Compiler::findReturn(const AST::Statement& s) {
  bool found = false;
  if (typeid(s) == typeid(AST::Return)){
    found = true;
  }
  if (typeid(s) == typeid(AST::IfStatement)){
    AST::IfStatement statement = dynamic_cast<const AST::IfStatement&>(s);
    found = findReturn(*statement.thenPart);
    if (statement.elsePart != nullptr){
      found = found && findReturn(*statement.elsePart);
    }
    else {
      found = false;
    }
  }
  if (typeid(s) == typeid(AST::WhileLoop)){
    AST::WhileLoop statement = dynamic_cast<const AST::WhileLoop&>(s);
    found = findReturn(*statement.body);
  }
  if (typeid(s) == typeid(AST::Block)){
    AST::Block statement = dynamic_cast<const AST::Block&>(s);
    if (statement.statements.size() > 0){
      for (const auto& state: statement.statements){
        found = found || findReturn(*state);
      }
    }
  }
  return found;
}

int Compiler::alloc_constant(Constant& constant) {
  int index = 0;
  for (Constant* c : enclosing_function_->getConstants()) {
    if (c->getType() == Value::Type::None && constant.getType() == Value::Type::None) {
      return index;
    }
    if (c->getType() == Value::Type::Integer && constant.getType() == Value::Type::Integer) {
      if (static_cast<Constant::Integer*>(c)->getValue() == static_cast<Constant::Integer*>(&constant)->getValue()) {
        return index;
      }
    }
    if (c->getType() == Value::Type::String && constant.getType() == Value::Type::String) {
      if (static_cast<Constant::String*>(c)->getValue() == static_cast<Constant::String*>(&constant)->getValue()) {
        return index;
      }
    }
    if (c->getType() == Value::Type::Boolean && constant.getType() == Value::Type::Boolean) {
      if (static_cast<Constant::Boolean*>(c)->getValue() == static_cast<Constant::Boolean*>(&constant)->getValue()) {
        return index;
      }
    }
    index += 1;
  }
  enclosing_function_->addConstant(&constant);
  return index;
}

int Compiler::alloc_name(const std::string& name) {
  int index = 0;
  for (std::string n : enclosing_function_->getNames()) {
    if (n == name) {
      return index;
    }
    index += 1;
  }
  enclosing_function_->addName(name);
  return index;
}