#include <iostream>
#include "antlr4-runtime.h"
#include "CST.h"
ExpressionNode::ExpressionNode(FunctionNode* fun_, BooleanNode* boolean_, RecordNode* record_)
    : fun(fun_), boolean(boolean_), record(record_) {}

std::string ExpressionNode::to_string() {
    #ifdef PRINT_DEBUG
    std::cout << "Entering ExpressionNode::to_string()\n";
    #endif

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

    #ifdef PRINT_DEBUG
    std::cout << "Exiting ExpressionNode::to_string()\n";
    #endif

    return ss.str();
}

// Function to report errors
void reportError(antlr4::Token& token) {
    std::stringstream str;
    str << "Unexpected Symbol (" << token.getLine() << ","
        << token.getCharPositionInLine() << "): " << token.getText() << "\n";
    throw std::runtime_error(str.str());
}

// ConstantNode constructor
ConstantNode::ConstantNode(antlr4::Token* token_) : value(token_) {}

std::string ConstantNode::to_string() {
    #ifdef PRINT_DEBUG
    std::cout << "Entering ConstantNode::to_string()\n";
    #endif

    std::stringstream ss;
    ss << "Constant(" << value->getText() << ")";

    #ifdef PRINT_DEBUG
    std::cout << "Exiting ConstantNode::to_string()\n";
    #endif

    return ss.str();
}

// UnitNode constructor
UnitNode::UnitNode(bool minus_,LHSNode* lhs_, ConstantNode* constant_, CallNode* call_, BooleanNode* boolean_)
    : minus(minus_), lhs(lhs_), constant(constant_), call(call_), boolean(boolean_) {}

std::string UnitNode::to_string() {
    #ifdef PRINT_DEBUG
    std::cout << "Entering UnitNode::to_string()\n";
    #endif

    std::stringstream ss;
    ss << "Unit(";
    if (minus) {
       ss << '-' ;
    } 
    if (lhs != nullptr){
        ss << lhs->to_string() << ")";
    }
    if (constant != nullptr){
        ss << constant->to_string() << ")";
    }
    if (call != nullptr){
        ss << call->to_string() << ")";
    }
    if (boolean != nullptr){
        ss << boolean->to_string() << ")";
    }

    #ifdef PRINT_DEBUG
    std::cout << "Exiting UnitNode::to_string()\n";
    #endif

    return ss.str();
}

// ProductPrimeNode constructor
ProductPrimeNode::ProductPrimeNode(antlr4::Token* op_token_, UnitNode* unit_, ProductPrimeNode* rest_)
    : op_token(op_token_), unit(unit_), rest(rest_) {}

std::string ProductPrimeNode::to_string() {
    #ifdef PRINT_DEBUG
    std::cout << "Entering ProductPrimeNode::to_string()\n";
    #endif

    std::stringstream ss;
    if (rest != nullptr) {
        ss << "ProductPrime(" << op_token->getText() << "," << unit->to_string() << ',' << rest->to_string() << ")";
    } else {
        ss << "ProductPrime(" << op_token->getText() << "," << unit->to_string() << ")";
    }

    #ifdef PRINT_DEBUG
    std::cout << "Exiting ProductPrimeNode::to_string()\n";
    #endif

    return ss.str();
}

// ProductNode constructor
ProductNode::ProductNode(UnitNode* unit_, ProductPrimeNode* rest_)
    : unit(unit_), rest(rest_) {}

std::string ProductNode::to_string() {
    #ifdef PRINT_DEBUG
    std::cout << "Entering ProductNode::to_string()\n";
    #endif

    std::stringstream ss;
    if (rest != nullptr) {
        ss << "ProductNode(" << unit->to_string() << ',' << rest->to_string() << ")";
    } else {
        ss << "ProductNode(" << unit->to_string() << ")";
    }

    #ifdef PRINT_DEBUG
    std::cout << "Exiting ProductNode::to_string()\n";
    #endif

    return ss.str();
}

// ArithmeticPrimeNode constructor
ArithmeticPrimeNode::ArithmeticPrimeNode(antlr4::Token* op_token_, ProductNode* prod_, ArithmeticPrimeNode* rest_)
    : op_token(op_token_), prod(prod_), rest(rest_) {}

std::string ArithmeticPrimeNode::to_string() {
    #ifdef PRINT_DEBUG
    std::cout << "Entering ArithmeticPrimeNode::to_string()\n";
    #endif

    std::stringstream ss;
    if (rest != nullptr) {
        ss << "ArithmeticPrime(" << op_token->getText() << "," << prod->to_string() << ',' << rest->to_string() << ")";
    } else {
        ss << "ArithmeticPrime(" << op_token->getText() << "," << prod->to_string() << ")";
    }

    #ifdef PRINT_DEBUG
    std::cout << "Exiting ArithmeticPrimeNode::to_string()\n";
    #endif

    return ss.str();
}

// ArithmeticNode constructor
ArithmeticNode::ArithmeticNode(ProductNode* prod_, ArithmeticPrimeNode* rest_)
    : prod(prod_), rest(rest_) {}

std::string ArithmeticNode::to_string() {
    #ifdef PRINT_DEBUG
    std::cout << "Entering ArithmeticNode::to_string()\n";
    #endif

    std::stringstream ss;
    if (rest != nullptr) {
        ss << "Arithmetic(" << prod->to_string() << ',' << rest->to_string() << ")";
    } else {
        ss << "Arithmetic(" << prod->to_string() << ")";
    }

    #ifdef PRINT_DEBUG
    std::cout << "Exiting ArithmeticNode::to_string()\n";
    #endif

    return ss.str();
}

// PredicateNode constructor
PredicateNode::PredicateNode(ArithmeticNode* arith_1_, antlr4::Token* cop_token_, ArithmeticNode* arith_2_)
    : arith_1(arith_1_), cop_token(cop_token_), arith_2(arith_2_) {}

std::string PredicateNode::to_string() {
    #ifdef PRINT_DEBUG
    std::cout << "Entering PredicateNode::to_string()\n";
    #endif

    std::stringstream ss;
    if (cop_token != nullptr && arith_2 != nullptr) {
        ss << "Predicate(" << arith_1->to_string() << ',' << cop_token->getText() << ',' << arith_2->to_string() << ")";
    } else {
        ss << "Predicate(" << arith_1->to_string() << ")";
    }

    #ifdef PRINT_DEBUG
    std::cout << "Exiting PredicateNode::to_string()\n";
    #endif

    return ss.str();
}

// BoolUnitNode constructor
BoolUnitNode::BoolUnitNode(bool negate_, PredicateNode* predicate_)
    : negate(negate_), predicate(predicate_) {}

std::string BoolUnitNode::to_string() {
    #ifdef PRINT_DEBUG
    std::cout << "Entering BoolUnitNode::to_string()\n";
    #endif

    std::stringstream ss;
    if (negate) {
        ss << "BoolUnit(!," << predicate->to_string() << ")";
    } else {
        ss << "BoolUnit(" << predicate->to_string() << ")";
    }

    #ifdef PRINT_DEBUG
    std::cout << "Exiting BoolUnitNode::to_string()\n";
    #endif

    return ss.str();
}

// ConjunctionPrimeNode constructor
ConjunctionPrimeNode::ConjunctionPrimeNode(antlr4::Token* op_token_, BoolUnitNode* bool_unit_, ConjunctionPrimeNode* rest_)
    : op_token(op_token_), bool_unit(bool_unit_), rest(rest_) {}

std::string ConjunctionPrimeNode::to_string() {
    #ifdef PRINT_DEBUG
    std::cout << "Entering ConjunctionPrimeNode::to_string()\n";
    #endif

    std::stringstream ss;
    if (rest != nullptr) {
        ss << "ConjunctionPrime(" << op_token->getText() << "," << bool_unit->to_string() << ',' << rest->to_string() << ")";
    } else {
        ss << "ConjunctionPrime(" << op_token->getText() << "," << bool_unit->to_string() << ")";
    }

    #ifdef PRINT_DEBUG
    std::cout << "Exiting ConjunctionPrimeNode::to_string()\n";
    #endif

    return ss.str();
}

// ConjunctionNode constructor
ConjunctionNode::ConjunctionNode(BoolUnitNode* bool_unit_, ConjunctionPrimeNode* rest_)
    : bool_unit(bool_unit_), rest(rest_) {}

std::string ConjunctionNode::to_string() {
    #ifdef PRINT_DEBUG
    std::cout << "Entering ConjunctionNode::to_string()\n";
    #endif

    std::stringstream ss;
    if (rest != nullptr) {
        ss << "Conjunction(" << bool_unit->to_string() << ',' << rest->to_string() << ")";
    } else {
        ss << "Conjunction(" << bool_unit->to_string() << ")";
    }

    #ifdef PRINT_DEBUG
    std::cout << "Exiting ConjunctionNode::to_string()\n";
    #endif

    return ss.str();
}
// Define PRINT_DEBUG in your build settings or uncomment the next line for testing
//#define PRINT_DEBUG

// BooleanPrimeNode constructor
BooleanPrimeNode::BooleanPrimeNode(antlr4::Token* op_token_, ConjunctionNode* conjunction_, BooleanPrimeNode* rest_)
    : op_token(op_token_), conjunction(conjunction_), rest(rest_) {}

std::string BooleanPrimeNode::to_string() {
    #ifdef PRINT_DEBUG
    std::cout << "Entering BooleanPrimeNode::to_string()\n";
    #endif

    std::stringstream ss;
    if (rest != nullptr) {
        ss << "BooleanPrime(" << op_token->getText() << "," << conjunction->to_string() << ',' << rest->to_string() << ")";
    } else {
        ss << "BooleanPrime(" << op_token->getText() << "," << conjunction->to_string() << ")";
    }

    #ifdef PRINT_DEBUG
    std::cout << "Exiting BooleanPrimeNode::to_string()\n";
    #endif

    return ss.str();
}

// BooleanNode constructor
BooleanNode::BooleanNode(ConjunctionNode* conjunction_, BooleanPrimeNode* rest_)
    : conjunction(conjunction_), rest(rest_) {}

std::string BooleanNode::to_string() {
    #ifdef PRINT_DEBUG
    std::cout << "Entering BooleanNode::to_string()\n";
    #endif

    std::stringstream ss;
    if (rest != nullptr) {
        ss << "Boolean(" << conjunction->to_string() << ',' << rest->to_string() << ")";
    } else {
        ss << "Boolean(" << conjunction->to_string() << ")";
    }

    #ifdef PRINT_DEBUG
    std::cout << "Exiting BooleanNode::to_string()\n";
    #endif

    return ss.str();
}

// RecordPrimeNode constructor and to_string method
RecordPrimeNode::RecordPrimeNode(antlr4::Token* name_, ExpressionNode* expr_, RecordPrimeNode* rest_)
    : name(name_), expr(expr_), rest(rest_) {}

std::string RecordPrimeNode::to_string() {
    #ifdef PRINT_DEBUG
    std::cout << "Entering RecordPrimeNode::to_string()\n";
    #endif

    std::stringstream ss;
    if (rest != nullptr) {
        ss << "RecordPrime(" << name->getText() << ',' << expr->to_string() << ',' << rest->to_string() << ')';
    } else {
        ss << "RecordPrime(" << name->getText() << ',' << expr->to_string() << ')';
    }

    #ifdef PRINT_DEBUG
    std::cout << "Exiting RecordPrimeNode::to_string()\n";
    #endif

    return ss.str();
}

// RecordNode constructor and to_string method
RecordNode::RecordNode(RecordPrimeNode* inside_) : inside(inside_) {}

std::string RecordNode::to_string() {
    #ifdef PRINT_DEBUG
    std::cout << "Entering RecordNode::to_string()\n";
    #endif

    std::stringstream ss;
    if (inside != nullptr) {
        ss << "Record(" << inside->to_string() << ")";
    } else {
        ss << "Record(" << ")";
    }

    #ifdef PRINT_DEBUG
    std::cout << "Exiting RecordNode::to_string()\n";
    #endif

    return ss.str();
}

// LHSPrimeNode constructor and to_string method
LHSPrimeNode::LHSPrimeNode(antlr4::Token* name_, ExpressionNode* expr_, LHSPrimeNode* rest_)
    : name(name_), expr(expr_), rest(rest_) {}

std::string LHSPrimeNode::to_string() {
    #ifdef PRINT_DEBUG
    std::cout << "Entering LHSPrimeNode::to_string()\n";
    #endif

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
            ss << "LHSPrime(" << name->getText()<< ')';
        }
        if (expr != nullptr) {
            ss << "LHSPrime(" << expr->to_string() << ')';
        }
    }

    #ifdef PRINT_DEBUG
    std::cout << "Exiting LHSPrimeNode::to_string()\n";
    #endif

    return ss.str();
}

// LHSNode constructor and to_string method
LHSNode::LHSNode(antlr4::Token* name_, LHSPrimeNode* inside_) : name(name_), inside(inside_) {}

std::string LHSNode::to_string() {
    #ifdef PRINT_DEBUG
    std::cout << "Entering LHSNode::to_string()\n";
    #endif

    std::stringstream ss;
    if (inside != nullptr) {
        ss << "LHS(" << name->getText() << ',' << inside->to_string() << ")";
    } else {
        ss << "LHS(" << name->getText() << ")";
    }

    #ifdef PRINT_DEBUG
    std::cout << "Exiting LHSNode::to_string()\n";
    #endif

    return ss.str();
}

// ArgumentPrimeNode constructor and to_string method
ArgumentPrimeNode::ArgumentPrimeNode(antlr4::Token* name_, ArgumentPrimeNode* rest_)
    : name(name_), rest(rest_) {}

std::string ArgumentPrimeNode::to_string() {
    #ifdef PRINT_DEBUG
    std::cout << "Entering ArgumentPrimeNode::to_string()\n";
    #endif

    std::stringstream ss;
    if (rest != nullptr) {
        ss << "ArgumentPrime(" << name->getText() << ',' << rest->to_string() << ")";
    } else {
        ss << "ArgumentPrime(" << name->getText() << ")";
    }

    #ifdef PRINT_DEBUG
    std::cout << "Exiting ArgumentPrimeNode::to_string()\n";
    #endif

    return ss.str();
}

// ArgumentNode constructor and to_string method
ArgumentNode::ArgumentNode(antlr4::Token* name_, ArgumentPrimeNode* rest_)
    : name(name_), rest(rest_) {}

std::string ArgumentNode::to_string() {
    #ifdef PRINT_DEBUG
    std::cout << "Entering ArgumentNode::to_string()\n";
    #endif

    std::stringstream ss;
    if (rest != nullptr) {
        ss << "Argument(" << name->getText() << ',' << rest->to_string() << ")";
    } else {
        ss << "Argument(" << name->getText() << ")";
    }

    #ifdef PRINT_DEBUG
    std::cout << "Exiting ArgumentNode::to_string()\n";
    #endif

    return ss.str();
}

// FunctionNode constructor and to_string method
FunctionNode::FunctionNode(ArgumentNode* args_, BlockNode* block_) : args(args_), block(block_) {}

std::string FunctionNode::to_string() {
    #ifdef PRINT_DEBUG
    std::cout << "Entering FunctionNode::to_string()\n";
    #endif

    std::stringstream ss;
    if (args != nullptr) {
        ss << "Function(" << args->to_string() << ',' << block->to_string() << ")";
    } else {
        ss << "Function(" << block->to_string() << ")";
    }

    #ifdef PRINT_DEBUG
    std::cout << "Exiting FunctionNode::to_string()\n";
    #endif

    return ss.str();
}

// ReturnNode constructor and to_string method
ReturnNode::ReturnNode(ExpressionNode* expr_) : expr(expr_) {}

std::string ReturnNode::to_string() {
    #ifdef PRINT_DEBUG
    std::cout << "Entering ReturnNode::to_string()\n";
    #endif

    std::stringstream ss;
    ss << "Return(" << expr->to_string() << ")";

    #ifdef PRINT_DEBUG
    std::cout << "Exiting ReturnNode::to_string()\n";
    #endif

    return ss.str();
}

// WhileNode constructor and to_string method
WhileNode::WhileNode(ExpressionNode* expr_, BlockNode* block_) : expr(expr_), block(block_) {}

std::string WhileNode::to_string() {
    #ifdef PRINT_DEBUG
    std::cout << "Entering WhileNode::to_string()\n";
    #endif

    std::stringstream ss;
    ss << "While(" << expr->to_string() << ',' << block->to_string() << ")";

    #ifdef PRINT_DEBUG
    std::cout << "Exiting WhileNode::to_string()\n";
    #endif

    return ss.str();
}

// ElseNode constructor and to_string method
ElseNode::ElseNode(BlockNode* block_) : block(block_) {}

std::string ElseNode::to_string() {
    #ifdef PRINT_DEBUG
    std::cout << "Entering ElseNode::to_string()\n";
    #endif

    std::stringstream ss;
    ss << "Else(" << block->to_string() << ")";

    #ifdef PRINT_DEBUG
    std::cout << "Exiting ElseNode::to_string()\n";
    #endif

    return ss.str();
}

// IfStatementNode constructor and to_string method
IfStatementNode::IfStatementNode(ExpressionNode* expr_, BlockNode* block_if_, ElseNode* else_block_)
    : expr(expr_), block_if(block_if_), else_block(else_block_) {}

std::string IfStatementNode::to_string() {
    #ifdef PRINT_DEBUG
    std::cout << "Entering IfStatementNode::to_string()\n";
    #endif

    std::stringstream ss;
    ss << "If(" << expr->to_string() << ',' << block_if->to_string() << ',' << (else_block != nullptr ? else_block->to_string() : "null") << ")";

    #ifdef PRINT_DEBUG
    std::cout << "Exiting IfStatementNode::to_string()\n";
    #endif

    return ss.str();
}

// BlockPrimeNode constructor and to_string method
BlockPrimeNode::BlockPrimeNode(StatementNode* statement_, BlockPrimeNode* rest_)
    : statement(statement_), rest(rest_) {}

std::string BlockPrimeNode::to_string() {
    #ifdef PRINT_DEBUG
    std::cout << "Entering BlockPrimeNode::to_string()\n";
    #endif

    std::stringstream ss;
    if (rest != nullptr) {
        ss << "BlockPrime(" << statement->to_string() << ',' << rest->to_string() << ")";
    } else {
        ss << "BlockPrime(" << statement->to_string() << ")";
    }

    #ifdef PRINT_DEBUG
    std::cout << "Exiting BlockPrimeNode::to_string()\n";
    #endif

    return ss.str();
}

// BlockNode class
BlockNode::BlockNode(BlockPrimeNode* inside_) : inside(inside_) {}

std::string BlockNode::to_string() {
    #ifdef PRINT_DEBUG
    std::cout << "Entering BlockNode::to_string()\n";
    #endif

    std::stringstream ss;
    ss << "Block(" << (inside != nullptr ? inside->to_string() : "null") << ")";

    #ifdef PRINT_DEBUG
    std::cout << "Exiting BlockNode::to_string()\n";
    #endif

    return ss.str();
}

// ParameterPrimeNode constructor and to_string method
ParameterPrimeNode::ParameterPrimeNode(ExpressionNode* expr_, ParameterPrimeNode* rest_)
    : expr(expr_), rest(rest_) {}

std::string ParameterPrimeNode::to_string() {
    #ifdef PRINT_DEBUG
    std::cout << "Entering ParameterPrimeNode::to_string()\n";
    #endif

    std::stringstream ss;
    if (rest != nullptr) {
        ss << "ParameterPrime(" << expr->to_string() << ',' << rest->to_string() << ")";
    } else {
        ss << "ParameterPrime(" << expr->to_string() << ")";
    }

    #ifdef PRINT_DEBUG
    std::cout << "Exiting ParameterPrimeNode::to_string()\n";
    #endif

    return ss.str();
}

// ParameterNode constructor and to_string method
ParameterNode::ParameterNode(ExpressionNode* expr_, ParameterPrimeNode* rest_)
    : expr(expr_), rest(rest_) {}

std::string ParameterNode::to_string() {
    #ifdef PRINT_DEBUG
    std::cout << "Entering ParameterNode::to_string()\n";
    #endif

    std::stringstream ss;
    if (rest != nullptr) {
        ss << "Parameter(" << expr->to_string() << ',' << rest->to_string() << ")";
    } else {
        ss << "Parameter(" << expr->to_string() << ")";
    }

    #ifdef PRINT_DEBUG
    std::cout << "Exiting ParameterNode::to_string()\n";
    #endif

    return ss.str();
}

// CallNode constructor and to_string method
CallNode::CallNode(LHSNode* lhs_, ParameterNode* arguments_) : lhs(lhs_), arguments(arguments_) {}

std::string CallNode::to_string() {
    #ifdef PRINT_DEBUG
    std::cout << "Entering CallNode::to_string()\n";
    #endif

    std::stringstream ss;
    if (arguments != nullptr) {
        ss << "Call(" << lhs->to_string() << ',' << arguments->to_string() << ")";
    } else {
        ss << "Call(" << lhs->to_string() << ")";
    }

    #ifdef PRINT_DEBUG
    std::cout << "Exiting CallNode::to_string()\n";
    #endif

    return ss.str();
}

// CallStatementNode constructor and to_string method
CallStatementNode::CallStatementNode(CallNode* call_) : call(call_) {}

std::string CallStatementNode::to_string() {
    #ifdef PRINT_DEBUG
    std::cout << "Entering CallStatementNode::to_string()\n";
    #endif

    std::stringstream ss;
    ss << "CallStatement(" << call->to_string() << ")";

    #ifdef PRINT_DEBUG
    std::cout << "Exiting CallStatementNode::to_string()\n";
    #endif

    return ss.str();
}

// AssignmentNode constructor and to_string method
AssignmentNode::AssignmentNode(LHSNode* lhs_, ExpressionNode* expr_)
    : lhs(lhs_), expr(expr_) {}

std::string AssignmentNode::to_string() {
    #ifdef PRINT_DEBUG
    std::cout << "Entering AssignmentNode::to_string()\n";
    #endif

    std::stringstream ss;
    ss << "AssignmentNode(" << lhs->to_string() << '=' << expr->to_string() << ")";

    #ifdef PRINT_DEBUG
    std::cout << "Exiting AssignmentNode::to_string()\n";
    #endif

    return ss.str();
}

// GlobalNode constructor and to_string method
GlobalNode::GlobalNode(antlr4::Token* name_) : name(name_) {}

std::string GlobalNode::to_string() {
    #ifdef PRINT_DEBUG
    std::cout << "Entering GlobalNode::to_string()\n";
    #endif

    std::stringstream ss;
    ss << "Global(" << name->getText() << ")";

    #ifdef PRINT_DEBUG
    std::cout << "Exiting GlobalNode::to_string()\n";
    #endif

    return ss.str();
}

// StatementNode constructor and to_string method
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
    #ifdef PRINT_DEBUG
    std::cout << "Entering StatementNode::to_string()\n";
    #endif

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

    #ifdef PRINT_DEBUG
    std::cout << "Exiting StatementNode::to_string()\n";
    #endif

    return ss.str();
}


StatementListNode::StatementListNode(StatementNode* statement_, StatementListNode* rest_)
    : statement(statement_), rest(rest_) {}

std::string StatementListNode::to_string() {
    #ifdef PRINT_DEBUG
    std::cout << "Entering StatementListNode::to_string()\n";
    #endif
    std::string result = "StatementList(";

    if (statement != nullptr) {
        result += statement->to_string(); // Assuming StatementNode has a to_string() method
        if (rest != nullptr) {
            result += ", "; // Add a semicolon if there's a rest
            result += rest->to_string(); // Recursively call to_string on the rest
        }
    }
    result += ")";
    #ifdef PRINT_DEBUG
    std::cout << "Exiting StatementListNode::to_string() with result: " << result << "\n";
    #endif
    return result;
}

// ProgramNode constructor and to_string method
ProgramNode::ProgramNode(StatementListNode* child_) : child(child_) {}

std::string ProgramNode::to_string() {
    #ifdef PRINT_DEBUG
    std::cout << "Entering ProgramNode::to_string()\n";
    #endif

    std::stringstream ss;
    ss << "Program(" << child->to_string() << ")";

    #ifdef PRINT_DEBUG
    std::cout << "Exiting ProgramNode::to_string()\n";
    #endif

    return ss.str();
}
