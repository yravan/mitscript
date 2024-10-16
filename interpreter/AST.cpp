#include "AST.h"
#include "Visitor.h"


void AST::Program::accept(Visitor &v) const {v.visit(*this);}
void AST::Expression::accept(Visitor &v) const {v.visit(*this);}
void AST::Statement::accept(Visitor &v) const {v.visit(*this);}
void AST::Assignment::accept(Visitor &v) const {v.visit(*this);}
void AST::Block::accept(Visitor &v) const {v.visit(*this);}
void AST::Global::accept(Visitor &v) const {v.visit(*this);}
void AST::CallStatement::accept(Visitor &v) const {v.visit(*this);}
void AST::IfStatement::accept(Visitor &v) const {v.visit(*this);}
void AST::WhileLoop::accept(Visitor &v) const {v.visit(*this);}
void AST::Return::accept(Visitor &v) const {v.visit(*this);}
void AST::FunctionDeclaration::accept(Visitor &v) const {v.visit(*this);}
void AST::BinaryExpression::accept(Visitor &v) const {v.visit(*this);}
void AST::UnaryExpression::accept(Visitor &v) const {v.visit(*this);}
void AST::FieldDereference::accept(Visitor &v) const {v.visit(*this);}
void AST::IndexExpression::accept(Visitor &v) const {v.visit(*this);}
void AST::Call::accept(Visitor &v) const {v.visit(*this);}
void AST::Record::accept(Visitor &v) const {v.visit(*this);}
void AST::IntegerConstant::accept(Visitor &v) const {v.visit(*this);}
void AST::StringConstant::accept(Visitor &v) const {v.visit(*this);}
void AST::NoneConstant::accept(Visitor &v) const {v.visit(*this);}
void AST::BooleanConstant::accept(Visitor &v) const {v.visit(*this);}


