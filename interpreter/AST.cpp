#include "AST.h"
#include "Visitor.h"

void AST_node::accept(Visitor& v) {
        v.visit(*this);
}

// Program constructor
Program::Program(Block* mainBlock_) : mainBlock(mainBlock_) {}

// Expression constructors
Expression::Expression(const std::string& name_) : name(name_) {}
Expression::Expression() : name("") {}

// Block constructor
Block::Block(const std::vector<Statement*>& stmts) : statements(stmts) {}

// Global constructor
Global::Global(const std::string& name_) : name(name_) {}

// Assignment constructor
Assignment::Assignment(Expression* lhs_, Expression* expr_) : lhs(lhs_), expr(expr_) {}

// CallStatement constructor
CallStatement::CallStatement(Call* callExpr_) : callExpr(callExpr_) {}

// IfStatement constructor
IfStatement::IfStatement(Expression* condition_, Block* thenPart_, Block* elsePart_)
    : condition(condition_), thenPart(thenPart_), elsePart(elsePart_) {}

// WhileLoop constructor
WhileLoop::WhileLoop(Expression* condition_, Block* body_)
    : condition(condition_), body(body_) {}

// Return constructor
Return::Return(Expression* expr_) : expr(expr_) {}

// FunctionDeclaration constructor
FunctionDeclaration::FunctionDeclaration(const std::vector<std::string>& args, Block* body_)
    : arguments(args), body(body_) {}

// BinaryExpression constructor
BinaryExpression::BinaryExpression(Expression* left_, const std::string& op_, Expression* right_)
    : left(left_), op(op_), right(right_) {}

// UnaryExpression constructor
UnaryExpression::UnaryExpression(Expression* operand_, const std::string& op_, bool preceding_)
    : operand(operand_), op(op_), preceding(preceding_) {}

// FieldDereference constructor
FieldDereference::FieldDereference(Expression* baseExpr_, const std::string& field_)
    : baseExpr(baseExpr_), field(field_) {}

// IndexExpression constructor
IndexExpression::IndexExpression(Expression* baseExpr_, Expression* index_)
    : baseExpr(baseExpr_), index(index_) {}

// Call constructor
Call::Call(Expression* targetExpr_, const std::vector<Expression*>& args)
    : targetExpr(targetExpr_), arguments(args) {}

// Record constructor
Record::Record(const std::map<std::string, Expression*>& fields_) : fields(fields_) {}

// IntegerConstant constructor
IntegerConstant::IntegerConstant(int value_) : value(value_) {}

// StringConstant constructor
StringConstant::StringConstant(const std::string& value_) : value(value_) {}

// NoneConstant constructor
NoneConstant::NoneConstant() {}

// BooleanConstant constructor
BooleanConstant::BooleanConstant(bool value_) : value(value_) {}
