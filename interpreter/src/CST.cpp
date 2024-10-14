/*
Written by me
*/
#include <iostream>
#include "antlr4-runtime.h"
#include "CST.h"


ExpressionNode::ExpressionNode(FunctionNode* fun_, BooleanNode* boolean_, RecordNode* record_)
    : fun(fun_), boolean(boolean_), record(record_) {}

std::string ExpressionNode::to_string() {
    DEBUG_PRINT("Entering ExpressionNode::to_string()");

    std::stringstream ss;
    if (this->fun != nullptr) {
        ss << "Expression(" << this->fun->to_string() << ")";
    }
    if (this->boolean != nullptr) {
        ss << "Expression(" << this->boolean->to_string() << ")";
    }
    if (this->record != nullptr) {
        ss << "Expression(" << this->record->to_string() << ")";
    }

    DEBUG_PRINT("Exiting ExpressionNode::to_string()");
    return ss.str();
}

void reportError(antlr4::Token& token) {
    std::stringstream str;
    str << "Unexpected Symbol (Line " << token.getLine() << ", Position "
        << token.getCharPositionInLine() << "): " << token.getText() << "\n";
    throw std::runtime_error(str.str());
}

ConstantNode::ConstantNode(antlr4::Token* token_) : value(token_) {}

std::string ConstantNode::to_string() {
    DEBUG_PRINT("Entering ConstantNode::to_string()");

    std::stringstream ss;
    ss << "Constant(" << value->getText() << ")";

    DEBUG_PRINT("Exiting ConstantNode::to_string()");
    return ss.str();
}

UnitNode::UnitNode(bool minus_, LHSNode* lhs_, ConstantNode* constant_, CallNode* call_, BooleanNode* boolean_)
    : minus(minus_), lhs(lhs_), constant(constant_), call(call_), boolean(boolean_) {}

std::string UnitNode::to_string() {
    DEBUG_PRINT("Entering UnitNode::to_string()");

    std::stringstream ss;
    ss << "Unit(";
    if (minus) {
        ss << '-';
    }
    if (lhs != nullptr) {
        ss << lhs->to_string() << ")";
    }
    if (constant != nullptr) {
        ss << constant->to_string() << ")";
    }
    if (call != nullptr) {
        ss << call->to_string() << ")";
    }
    if (boolean != nullptr) {
        ss << boolean->to_string() << ")";
    }

    DEBUG_PRINT("Exiting UnitNode::to_string()");
    return ss.str();
}

ProductPrimeNode::ProductPrimeNode(antlr4::Token* op_token_, UnitNode* unit_, ProductPrimeNode* rest_)
    : op_token(op_token_), unit(unit_), rest(rest_) {}

std::string ProductPrimeNode::to_string() {
    DEBUG_PRINT("Entering ProductPrimeNode::to_string()");

    std::stringstream ss;
    if (rest != nullptr) {
        ss << "ProductPrime(" << op_token->getText() << "," << unit->to_string() << ',' << rest->to_string() << ")";
    } else {
        ss << "ProductPrime(" << op_token->getText() << "," << unit->to_string() << ")";
    }

    DEBUG_PRINT("Exiting ProductPrimeNode::to_string()");
    return ss.str();
}

ProductNode::ProductNode(UnitNode* unit_, ProductPrimeNode* rest_)
    : unit(unit_), rest(rest_) {}

std::string ProductNode::to_string() {
    DEBUG_PRINT("Entering ProductNode::to_string()");

    std::stringstream ss;
    if (rest != nullptr) {
        ss << "ProductNode(" << unit->to_string() << ',' << rest->to_string() << ")";
    } else {
        ss << "ProductNode(" << unit->to_string() << ")";
    }

    DEBUG_PRINT("Exiting ProductNode::to_string()");
    return ss.str();
}

ArithmeticPrimeNode::ArithmeticPrimeNode(antlr4::Token* op_token_, ProductNode* prod_, ArithmeticPrimeNode* rest_)
    : op_token(op_token_), prod(prod_), rest(rest_) {}

std::string ArithmeticPrimeNode::to_string() {
    DEBUG_PRINT("Entering ArithmeticPrimeNode::to_string()");

    std::stringstream ss;
    if (rest != nullptr) {
        ss << "ArithmeticPrime(" << op_token->getText() << "," << prod->to_string() << ',' << rest->to_string() << ")";
    } else {
        ss << "ArithmeticPrime(" << op_token->getText() << "," << prod->to_string() << ")";
    }

    DEBUG_PRINT("Exiting ArithmeticPrimeNode::to_string()");
    return ss.str();
}

ArithmeticNode::ArithmeticNode(ProductNode* prod_, ArithmeticPrimeNode* rest_)
    : prod(prod_), rest(rest_) {}

std::string ArithmeticNode::to_string() {
    DEBUG_PRINT("Entering ArithmeticNode::to_string()");

    std::stringstream ss;
    if (rest != nullptr) {
        ss << "Arithmetic(" << prod->to_string() << ',' << rest->to_string() << ")";
    } else {
        ss << "Arithmetic(" << prod->to_string() << ")";
    }

    DEBUG_PRINT("Exiting ArithmeticNode::to_string()");
    return ss.str();
}

PredicateNode::PredicateNode(ArithmeticNode* arith_1_, antlr4::Token* cop_token_, ArithmeticNode* arith_2_)
    : arith_1(arith_1_), cop_token(cop_token_), arith_2(arith_2_) {}

std::string PredicateNode::to_string() {
    DEBUG_PRINT("Entering PredicateNode::to_string()");

    std::stringstream ss;
    if (cop_token != nullptr && arith_2 != nullptr) {
        ss << "Predicate(" << arith_1->to_string() << ',' << cop_token->getText() << ',' << arith_2->to_string() << ")";
    } else {
        ss << "Predicate(" << arith_1->to_string() << ")";
    }

    DEBUG_PRINT("Exiting PredicateNode::to_string()");
    return ss.str();
}

BoolUnitNode::BoolUnitNode(bool negate_, PredicateNode* predicate_)
    : negate(negate_), predicate(predicate_) {}

std::string BoolUnitNode::to_string() {
    DEBUG_PRINT("Entering BoolUnitNode::to_string()");

    std::stringstream ss;
    if (negate) {
        ss << "BoolUnit(!," << predicate->to_string() << ")";
    } else {
        ss << "BoolUnit(" << predicate->to_string() << ")";
    }

    DEBUG_PRINT("Exiting BoolUnitNode::to_string()");
    return ss.str();
}

ConjunctionPrimeNode::ConjunctionPrimeNode(antlr4::Token* op_token_, BoolUnitNode* bool_unit_, ConjunctionPrimeNode* rest_)
    : op_token(op_token_), bool_unit(bool_unit_), rest(rest_) {}

std::string ConjunctionPrimeNode::to_string() {
    DEBUG_PRINT("Entering ConjunctionPrimeNode::to_string()");

    std::stringstream ss;
    if (rest != nullptr) {
        ss << "ConjunctionPrime(" << op_token->getText() << "," << bool_unit->to_string() << ',' << rest->to_string() << ")";
    } else {
        ss << "ConjunctionPrime(" << op_token->getText() << "," << bool_unit->to_string() << ")";
    }

    DEBUG_PRINT("Exiting ConjunctionPrimeNode::to_string()");
    return ss.str();
}

ConjunctionNode::ConjunctionNode(BoolUnitNode* bool_unit_, ConjunctionPrimeNode* rest_)
    : bool_unit(bool_unit_), rest(rest_) {}

std::string ConjunctionNode::to_string() {
    DEBUG_PRINT("Entering ConjunctionNode::to_string()");

    std::stringstream ss;
    if (rest != nullptr) {
        ss << "Conjunction(" << bool_unit->to_string() << ',' << rest->to_string() << ")";
    } else {
        ss << "Conjunction(" << bool_unit->to_string() << ")";
    }

    DEBUG_PRINT("Exiting ConjunctionNode::to_string()");
    return ss.str();
}

BooleanPrimeNode::BooleanPrimeNode(antlr4::Token* op_token_, ConjunctionNode* conjunction_, BooleanPrimeNode* rest_)
    : op_token(op_token_), conjunction(conjunction_), rest(rest_) {}

std::string BooleanPrimeNode::to_string() {
    DEBUG_PRINT("Entering BooleanPrimeNode::to_string()");

    std::stringstream ss;
    if (rest != nullptr) {
        ss << "BooleanPrime(" << op_token->getText() << "," << conjunction->to_string() << ',' << rest->to_string() << ")";
    } else {
        ss << "BooleanPrime(" << op_token->getText() << "," << conjunction->to_string() << ")";
    }

    DEBUG_PRINT("Exiting BooleanPrimeNode::to_string()");
    return ss.str();
}

BooleanNode::BooleanNode(ConjunctionNode* conjunction_, BooleanPrimeNode* rest_)
    : conjunction(conjunction_), rest(rest_) {}

std::string BooleanNode::to_string() {
    DEBUG_PRINT("Entering BooleanNode::to_string()");

    std::stringstream ss;
    if (rest != nullptr) {
        ss << "Boolean(" << conjunction->to_string() << ',' << rest->to_string() << ")";
    } else {
        ss << "Boolean(" << conjunction->to_string() << ")";
    }

    DEBUG_PRINT("Exiting BooleanNode::to_string()");
    return ss.str();
}

RecordPrimeNode::RecordPrimeNode(antlr4::Token* name_, ExpressionNode* expr_, RecordPrimeNode* rest_)
    : name(name_), expr(expr_), rest(rest_) {}

std::string RecordPrimeNode::to_string() {
    DEBUG_PRINT("Entering RecordPrimeNode::to_string()");

    std::stringstream ss;
    if (rest != nullptr) {
        ss << "RecordPrime(" << name->getText() << ',' << expr->to_string() << ',' << rest->to_string() << ')';
    } else {
        ss << "RecordPrime(" << name->getText() << ',' << expr->to_string() << ')';
    }

    DEBUG_PRINT("Exiting RecordPrimeNode::to_string()");
    return ss.str();
}

RecordNode::RecordNode(RecordPrimeNode* inside_) : inside(inside_) {}

std::string RecordNode::to_string() {
    DEBUG_PRINT("Entering RecordNode::to_string()");

    std::stringstream ss;
    if (inside != nullptr) {
        ss << "Record(" << inside->to_string() << ")";
    } else {
        ss << "Record(" << ")";
    }

    DEBUG_PRINT("Exiting RecordNode::to_string()");
    return ss.str();
}

LHSPrimeNode::LHSPrimeNode(antlr4::Token* name_, ExpressionNode* expr_, LHSPrimeNode* rest_)
    : name(name_), expr(expr_), rest(rest_) {}

std::string LHSPrimeNode::to_string() {
    DEBUG_PRINT("Entering LHSPrimeNode::to_string()");

    std::stringstream ss;
    if (rest != nullptr) {
        if (name != nullptr) {
            ss << "LHSPrime(" << name->getText() << ',' << rest->to_string() << ')';
        }
        if (expr != nullptr) {
            ss << "LHSPrime(" << expr->to_string() << ',' << rest->to_string() << ')';
        }
    } else {
        if (name != nullptr) {
            ss << "LHSPrime(" << name->getText() << ')';
        }
        if (expr != nullptr) {
            ss << "LHSPrime(" << expr->to_string() << ')';
        }
    }

    DEBUG_PRINT("Exiting LHSPrimeNode::to_string()");
    return ss.str();
}

LHSNode::LHSNode(antlr4::Token* name_, LHSPrimeNode* inside_) : name(name_), inside(inside_) {}

std::string LHSNode::to_string() {
    DEBUG_PRINT("Entering LHSNode::to_string()");

    std::stringstream ss;
    if (inside != nullptr) {
        ss << "LHS(" << name->getText() << ',' << inside->to_string() << ")";
    } else {
        ss << "LHS(" << name->getText() << ")";
    }

    DEBUG_PRINT("Exiting LHSNode::to_string()");
    return ss.str();
}

ArgumentPrimeNode::ArgumentPrimeNode(antlr4::Token* name_, ArgumentPrimeNode* rest_)
    : name(name_), rest(rest_) {}

std::string ArgumentPrimeNode::to_string() {
    DEBUG_PRINT("Entering ArgumentPrimeNode::to_string()");

    std::stringstream ss;
    if (rest != nullptr) {
        ss << "ArgumentPrime(" << name->getText() << ',' << rest->to_string() << ")";
    } else {
        ss << "ArgumentPrime(" << name->getText() << ")";
    }

    DEBUG_PRINT("Exiting ArgumentPrimeNode::to_string()");
    return ss.str();
}

ArgumentNode::ArgumentNode(antlr4::Token* name_, ArgumentPrimeNode* rest_)
    : name(name_), rest(rest_) {}

std::string ArgumentNode::to_string() {
    DEBUG_PRINT("Entering ArgumentNode::to_string()");

    std::stringstream ss;
    if (rest != nullptr) {
        ss << "Argument(" << name->getText() << ',' << rest->to_string() << ")";
    } else {
        ss << "Argument(" << name->getText() << ")";
    }

    DEBUG_PRINT("Exiting ArgumentNode::to_string()");
    return ss.str();
}

FunctionNode::FunctionNode(ArgumentNode* args_, BlockNode* block_) : args(args_), block(block_) {}

std::string FunctionNode::to_string() {
    DEBUG_PRINT("Entering FunctionNode::to_string()");

    std::stringstream ss;
    if (args != nullptr) {
        ss << "Function(" << args->to_string() << ',' << block->to_string() << ")";
    } else {
        ss << "Function(" << block->to_string() << ")";
    }

    DEBUG_PRINT("Exiting FunctionNode::to_string()");
    return ss.str();
}

ReturnNode::ReturnNode(ExpressionNode* expr_) : expr(expr_) {}

std::string ReturnNode::to_string() {
    DEBUG_PRINT("Entering ReturnNode::to_string()");

    std::stringstream ss;
    ss << "Return(" << expr->to_string() << ")";

    DEBUG_PRINT("Exiting ReturnNode::to_string()");
    return ss.str();
}

WhileNode::WhileNode(ExpressionNode* expr_, BlockNode* block_) : expr(expr_), block(block_) {}

std::string WhileNode::to_string() {
    DEBUG_PRINT("Entering WhileNode::to_string()");

    std::stringstream ss;
    ss << "While(" << expr->to_string() << ',' << block->to_string() << ")";

    DEBUG_PRINT("Exiting WhileNode::to_string()");
    return ss.str();
}

ElseNode::ElseNode(BlockNode* block_) : block(block_) {}

std::string ElseNode::to_string() {
    DEBUG_PRINT("Entering ElseNode::to_string()");

    std::stringstream ss;
    ss << "Else(" << block->to_string() << ")";

    DEBUG_PRINT("Exiting ElseNode::to_string()");
    return ss.str();
}

IfStatementNode::IfStatementNode(ExpressionNode* expr_, BlockNode* block_if_, ElseNode* else_block_)
    : expr(expr_), block_if(block_if_), else_block(else_block_) {}

std::string IfStatementNode::to_string() {
    DEBUG_PRINT("Entering IfStatementNode::to_string()");

    std::stringstream ss;
    ss << "If(" << expr->to_string() << ',' << block_if->to_string() << ',' << (else_block != nullptr ? else_block->to_string() : "null") << ")";

    DEBUG_PRINT("Exiting IfStatementNode::to_string()");
    return ss.str();
}

BlockPrimeNode::BlockPrimeNode(StatementNode* statement_, BlockPrimeNode* rest_)
    : statement(statement_), rest(rest_) {}

std::string BlockPrimeNode::to_string() {
    DEBUG_PRINT("Entering BlockPrimeNode::to_string()");

    std::stringstream ss;
    if (rest != nullptr) {
        ss << "BlockPrime(" << statement->to_string() << ',' << rest->to_string() << ")";
    } else {
        ss << "BlockPrime(" << statement->to_string() << ")";
    }

    DEBUG_PRINT("Exiting BlockPrimeNode::to_string()");
    return ss.str();
}

BlockNode::BlockNode(BlockPrimeNode* inside_) : inside(inside_) {}

std::string BlockNode::to_string() {
    DEBUG_PRINT("Entering BlockNode::to_string()");

    std::stringstream ss;
    ss << "Block(" << (inside != nullptr ? inside->to_string() : "null") << ")";

    DEBUG_PRINT("Exiting BlockNode::to_string()");
    return ss.str();
}

ParameterPrimeNode::ParameterPrimeNode(ExpressionNode* expr_, ParameterPrimeNode* rest_)
    : expr(expr_), rest(rest_) {}

std::string ParameterPrimeNode::to_string() {
    DEBUG_PRINT("Entering ParameterPrimeNode::to_string()");

    std::stringstream ss;
    if (rest != nullptr) {
        ss << "ParameterPrime(" << expr->to_string() << ',' << rest->to_string() << ")";
    } else {
        ss << "ParameterPrime(" << expr->to_string() << ")";
    }

    DEBUG_PRINT("Exiting ParameterPrimeNode::to_string()");
    return ss.str();
}

ParameterNode::ParameterNode(ExpressionNode* expr_, ParameterPrimeNode* rest_)
    : expr(expr_), rest(rest_) {}

std::string ParameterNode::to_string() {
    DEBUG_PRINT("Entering ParameterNode::to_string()");

    std::stringstream ss;
    if (rest != nullptr) {
        ss << "Parameter(" << expr->to_string() << ',' << rest->to_string() << ")";
    } else {
        ss << "Parameter(" << expr->to_string() << ")";
    }

    DEBUG_PRINT("Exiting ParameterNode::to_string()");
    return ss.str();
}

CallNode::CallNode(LHSNode* lhs_, ParameterNode* arguments_) : lhs(lhs_), arguments(arguments_) {}

std::string CallNode::to_string() {
    DEBUG_PRINT("Entering CallNode::to_string()");

    std::stringstream ss;
    if (arguments != nullptr) {
        ss << "Call(" << lhs->to_string() << ',' << arguments->to_string() << ")";
    } else {
        ss << "Call(" << lhs->to_string() << ")";
    }

    DEBUG_PRINT("Exiting CallNode::to_string()");
    return ss.str();
}

CallStatementNode::CallStatementNode(CallNode* call_) : call(call_) {}

std::string CallStatementNode::to_string() {
    DEBUG_PRINT("Entering CallStatementNode::to_string()");

    std::stringstream ss;
    ss << "CallStatement(" << call->to_string() << ")";

    DEBUG_PRINT("Exiting CallStatementNode::to_string()");
    return ss.str();
}

AssignmentNode::AssignmentNode(LHSNode* lhs_, ExpressionNode* expr_)
    : lhs(lhs_), expr(expr_) {}

std::string AssignmentNode::to_string() {
    DEBUG_PRINT("Entering AssignmentNode::to_string()");

    std::stringstream ss;
    ss << "AssignmentNode(" << lhs->to_string() << '=' << expr->to_string() << ")";

    DEBUG_PRINT("Exiting AssignmentNode::to_string()");
    return ss.str();
}

GlobalNode::GlobalNode(antlr4::Token* name_) : name(name_) {}

std::string GlobalNode::to_string() {
    DEBUG_PRINT("Entering GlobalNode::to_string()");

    std::stringstream ss;
    ss << "Global(" << name->getText() << ")";

    DEBUG_PRINT("Exiting GlobalNode::to_string()");
    return ss.str();
}

StatementNode::StatementNode(AssignmentNode* assignment_,
                             CallStatementNode* callStatement_,
                             GlobalNode* global_,
                             IfStatementNode* ifStatement_,
                             WhileNode* whileLoop_,
                             ReturnNode* return_)
    : assignment(assignment_), callStatement(callStatement_),
      global(global_), ifStatement(ifStatement_), whileLoop(whileLoop_),
      returnNode(return_) {}

std::string StatementNode::to_string() {
    DEBUG_PRINT("Entering StatementNode::to_string()");

    std::stringstream ss;
    if (assignment != nullptr) {
        ss << "Statement(" << assignment->to_string() << ")";
    }
    if (callStatement != nullptr) {
        ss << "Statement(" << callStatement->to_string() << ")";
    }
    if (global != nullptr) {
        ss << "Statement(" << global->to_string() << ")";
    }
    if (ifStatement != nullptr) {
        ss << "Statement(" << ifStatement->to_string() << ")";
    }
    if (whileLoop != nullptr) {
        ss << "Statement(" << whileLoop->to_string() << ")";
    }
    if (returnNode != nullptr) {
        ss << "Statement(" << returnNode->to_string() << ")";
    }

    DEBUG_PRINT("Exiting StatementNode::to_string()");
    return ss.str();
}

StatementListNode::StatementListNode(StatementNode* statement_, StatementListNode* rest_)
    : statement(statement_), rest(rest_) {}

std::string StatementListNode::to_string() {
    DEBUG_PRINT("Entering StatementListNode::to_string()");
    std::string result = "StatementList(";

    if (statement != nullptr) {
        result += statement->to_string();
        if (rest != nullptr) {
            result += ", ";
            result += rest->to_string();
        }
    }
    result += ")";
    DEBUG_PRINT("Exiting StatementListNode::to_string() with result: " + result);
    return result;
}

ProgramNode::ProgramNode(StatementListNode* child_) : child(child_) {}

std::string ProgramNode::to_string() {
    DEBUG_PRINT("Entering ProgramNode::to_string()");

    std::stringstream ss;
    if (child != nullptr){
        ss << "Program(" << child->to_string() << ")";
    }
    else{
        ss << "Program(" << ")";
    }

    DEBUG_PRINT("Exiting ProgramNode::to_string()");
    return ss.str();
}

