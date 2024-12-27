#include "AST.h"
#include "Visitor.h"


AST::Program::~Program() {
    delete mainBlock;
}
AST::Block::~Block() {
    for (Statement* stmt : statements) {
        delete stmt;
    }
}
AST::Assignment::~Assignment() {
    delete lhs;
    delete expr;
}
AST::CallStatement::~CallStatement() {
    delete callExpr;
}
AST::IfStatement::~IfStatement() {
    delete condition;
    delete thenPart;
    delete elsePart;
}
AST::WhileLoop::~WhileLoop() {
    delete condition;
    delete body;
}
AST::Return::~Return() {
    delete expr;
}
AST::FunctionDeclaration::~FunctionDeclaration() {
    delete body;
}
AST::BinaryExpression::~BinaryExpression() {
    delete left;
    delete right;
}
AST::UnaryExpression::~UnaryExpression() {
    delete operand;
}
AST::FieldDereference::~FieldDereference() {
    delete baseExpr;
}
AST::IndexExpression::~IndexExpression() {
    delete baseExpr;
    delete index;
}
AST::Call::~Call() {
    for (Expression* arg : arguments) {
        delete arg;
    }
}
AST::Record::~Record() {
    for (const auto& field : fields) {
        delete field.second;
    }
}

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

bool AST::Program::equals(AST_node& other) {
    Program* otherProgram = dynamic_cast<Program*>(&other);
    if (!otherProgram) return false;
    return this->mainBlock && this->mainBlock->equals(*otherProgram->mainBlock);
}

bool AST::Expression::equals(AST_node& other) {
    Expression* otherExpr = dynamic_cast<Expression*>(&other);
    if (!otherExpr) return false;
    return this->name == otherExpr->name;
}

bool AST::Statement::equals(AST_node& other) {
    Statement* otherStmt = dynamic_cast<Statement*>(&other);
    return otherStmt != nullptr;  // Base statement check
}

bool AST::Block::equals(AST_node& other) {
    Block* otherBlock = dynamic_cast<Block*>(&other);
    if (!otherBlock) return false;
    if (this->statements.size() != otherBlock->statements.size()) return false;
    for (size_t i = 0; i < this->statements.size(); ++i) {
        if (!this->statements[i] || !otherBlock->statements[i]) return false;
        if (!this->statements[i]->equals(*otherBlock->statements[i])) return false;
    }
    return true;
}

bool AST::Global::equals(AST_node& other) {
    Global* otherGlobal = dynamic_cast<Global*>(&other);
    if (!otherGlobal) return false;
    return this->name == otherGlobal->name;
}

bool AST::Assignment::equals(AST_node& other) {
    Assignment* otherAssign = dynamic_cast<Assignment*>(&other);
    if (!otherAssign) return false;
    return this->lhs && this->lhs->equals(*otherAssign->lhs) && this->expr && this->expr->equals(*otherAssign->expr);
}

bool AST::CallStatement::equals(AST_node& other) {
    CallStatement* otherCallStmt = dynamic_cast<CallStatement*>(&other);
    if (!otherCallStmt) return false;
    return this->callExpr && this->callExpr->equals(*otherCallStmt->callExpr);
}

bool AST::IfStatement::equals(AST_node& other) {
    IfStatement* otherIfStmt = dynamic_cast<IfStatement*>(&other);
    if (!otherIfStmt) return false;
    if (!this->condition || !this->condition->equals(*otherIfStmt->condition)) return false;
    if (!this->thenPart || !this->thenPart->equals(*otherIfStmt->thenPart)) return false;
    if ((this->elsePart == nullptr && otherIfStmt->elsePart != nullptr) || 
        (this->elsePart != nullptr && !this->elsePart->equals(*otherIfStmt->elsePart))) return false;
    return true;
}

bool AST::WhileLoop::equals(AST_node& other) {
    WhileLoop* otherLoop = dynamic_cast<WhileLoop*>(&other);
    if (!otherLoop) return false;
    return this->condition && this->condition->equals(*otherLoop->condition) && this->body && this->body->equals(*otherLoop->body);
}

bool AST::Return::equals(AST_node& other) {
    Return* otherReturn = dynamic_cast<Return*>(&other);
    if (!otherReturn) return false;
    return this->expr && this->expr->equals(*otherReturn->expr);
}

bool AST::FunctionDeclaration::equals(AST_node& other) {
    FunctionDeclaration* otherFunc = dynamic_cast<FunctionDeclaration*>(&other);
    if (!otherFunc) return false;
    if (this->arguments != otherFunc->arguments) return false;
    return this->body && this->body->equals(*otherFunc->body);
}

bool AST::BinaryExpression::equals(AST_node& other) {
    BinaryExpression* otherExpr = dynamic_cast<BinaryExpression*>(&other);
    if (!otherExpr) return false;
    return this->left && this->left->equals(*otherExpr->left) && this->op == otherExpr->op && this->right && this->right->equals(*otherExpr->right);
}

bool AST::UnaryExpression::equals(AST_node& other) {
    UnaryExpression* otherExpr = dynamic_cast<UnaryExpression*>(&other);
    if (!otherExpr) return false;
    return this->operand && this->operand->equals(*otherExpr->operand) && this->op == otherExpr->op;
}

bool AST::FieldDereference::equals(AST_node& other) {
    FieldDereference* otherField = dynamic_cast<FieldDereference*>(&other);
    if (!otherField) return false;
    return this->baseExpr && this->baseExpr->equals(*otherField->baseExpr) && this->field == otherField->field;
}

bool AST::IndexExpression::equals(AST_node& other) {
    IndexExpression* otherIndex = dynamic_cast<IndexExpression*>(&other);
    if (!otherIndex) return false;
    return this->baseExpr && this->baseExpr->equals(*otherIndex->baseExpr) && this->index && this->index->equals(*otherIndex->index);
}

bool AST::Call::equals(AST_node& other) {
    Call* otherCall = dynamic_cast<Call*>(&other);
    if (!otherCall) return false;
    if (!this->targetExpr || !this->targetExpr->equals(*otherCall->targetExpr)) return false;
    if (this->arguments.size() != otherCall->arguments.size()) return false;
    for (size_t i = 0; i < this->arguments.size(); ++i) {
        if (!this->arguments[i] || !otherCall->arguments[i]) return false;
        if (!this->arguments[i]->equals(*otherCall->arguments[i])) return false;
    }
    return true;
}

bool AST::Record::equals(AST_node& other) {
    Record* otherRecord = dynamic_cast<Record*>(&other);
    if (!otherRecord) return false;
    if (this->fields.size() != otherRecord->fields.size()) return false;
    for (size_t i = 0; i < this->fields.size(); ++i) {
        if (!this->fields[i].second || !otherRecord->fields[i].second) return false;
        if (this->fields[i].first != otherRecord->fields[i].first || !this->fields[i].second->equals(*otherRecord->fields[i].second)) return false;
    }
    return true;
}

bool AST::IntegerConstant::equals(AST_node& other) {
    IntegerConstant* otherInt = dynamic_cast<IntegerConstant*>(&other);
    if (!otherInt) return false;
    return this->value == otherInt->value;
}

bool AST::StringConstant::equals(AST_node& other) {
    StringConstant* otherStr = dynamic_cast<StringConstant*>(&other);
    if (!otherStr) return false;
    return this->value == otherStr->value;
}

bool AST::NoneConstant::equals(AST_node& other) {
    NoneConstant* otherNone = dynamic_cast<NoneConstant*>(&other);
    return otherNone != nullptr;  // Always equal, since NoneConstant has no fields
}

bool AST::BooleanConstant::equals(AST_node& other) {
    BooleanConstant* otherBool = dynamic_cast<BooleanConstant*>(&other);
    if (!otherBool) return false;
    return this->value == otherBool->value;
}



