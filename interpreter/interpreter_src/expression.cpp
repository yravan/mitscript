#include "debug.h"
#include "interpreter.h"

Value* Interpreter::evalPlus(const AST::BinaryExpression& expr){
    DEBUG_PRINT("Entering evalPlus with ");
    DEBUG_PRINT_AST(expr, printer);

    expr.left->accept(*this);
    Value* left_val = this->rval;
    expr.right->accept(*this);
    Value* right_val = this->rval;

    if (typeid(*left_val) == typeid(String) || typeid(*right_val) == typeid(String) ){
        DEBUG_PRINT("Exiting evalPlus");
        return stringConcatenation(left_val, right_val);
    }
    DEBUG_PRINT("Exiting evalPlus");
    return arithmeticOperation(left_val, right_val);

}

Bool* Interpreter::logicalOperation(const AST::BinaryExpression& expr) {
    DEBUG_PRINT("Entering logicalOperation with ");
    DEBUG_PRINT_AST(expr, printer);

    expr.left->accept(*this);
    Value* left_val = this->rval;
    expr.right->accept(*this);
    Value* right_val = this->rval;

    if (typeid(*left_val) != typeid(Bool)) {
        throw IllegalCastException(this->operator_to_string(expr.op), left_val);
    }
    if (typeid(*right_val) != typeid(Bool)) {
        throw IllegalCastException(this->operator_to_string(expr.op), right_val);
    }

    Bool* left_bool = dynamic_cast<Bool*>(left_val);
    Bool* right_bool = dynamic_cast<Bool*>(right_val);

    if (expr.op == AST::Operator::OR) {
        DEBUG_PRINT("Exiting logicalOperation" );
        return new Bool(left_bool->val || right_bool->val);
    }
    if (expr.op == AST::Operator::AND) {
        DEBUG_PRINT("Exiting logicalOperation" );
        return new Bool(left_bool->val && right_bool->val);
    }
    return nullptr; // Just in case no valid operator is found
}

Integer* Interpreter::arithmeticOperation(const AST::BinaryExpression& expr) {
    DEBUG_PRINT("Entering arithmeticOperation with " );
    DEBUG_PRINT_AST(expr, printer);

    expr.left->accept(*this);
    Value* left = this->rval;
    expr.right->accept(*this);
    Value* right = this->rval;

    if (typeid(*left) != typeid(Integer)) {
        throw IllegalCastException(this->operator_to_string(expr.op), left);
    }
    if (typeid(*right) != typeid(Integer)) {
        throw IllegalCastException(this->operator_to_string(expr.op), right);
    }

    Integer* left_int = dynamic_cast<Integer*>(left);
    Integer* right_int = dynamic_cast<Integer*>(right);

    if (expr.op == AST::Operator::PLUS) {
        DEBUG_PRINT("Exiting arithmeticOperation" );
        return new Integer(left_int->val + right_int->val);
    }
    if (expr.op == AST::Operator::SUB) {
        DEBUG_PRINT("Exiting arithmeticOperation" );
        return new Integer(left_int->val - right_int->val);
    }
    if (expr.op == AST::Operator::MUL) {
        DEBUG_PRINT("Exiting arithmeticOperation" );
        return new Integer(left_int->val * right_int->val);
    }
    if (expr.op == AST::Operator::DIV) {
        if (right_int->val == 0) {
            throw IllegalArithmeticException();
        }
        DEBUG_PRINT("Exiting arithmeticOperation" );
        return new Integer(left_int->val / right_int->val);
    }
    return nullptr; // Default return for unrecognized operators
}

// overloaded version for plus
Integer* Interpreter::arithmeticOperation(Value* left, Value* right) {
    DEBUG_PRINT("Entering arithmeticOperation with " );

    if (typeid(*left) != typeid(Integer)) {
        throw IllegalCastException("+", left);
    }
    if (typeid(*right) != typeid(Integer)) {
        throw IllegalCastException("+", right);
    }

    Integer* left_int = dynamic_cast<Integer*>(left);
    Integer* right_int = dynamic_cast<Integer*>(right);

    DEBUG_PRINT("Exiting arithmeticOperation" );
    return new Integer(left_int->val + right_int->val);
}

Value* Interpreter::unaryOperation(const AST::UnaryExpression& expr) {
    DEBUG_PRINT("Entering unaryOperation with " );
    DEBUG_PRINT_AST(expr, printer);
    
    expr.operand->accept(*this);
    Value* operand = this->rval;

    if (expr.op == AST::Operator::SUB) {
        if (typeid(*operand) != typeid(Integer)) {
            throw IllegalCastException(this->operator_to_string(expr.op), operand);
        }
        Integer* operand_int = dynamic_cast<Integer*>(operand);
        DEBUG_PRINT("Exiting unaryOperation" );
        return new Integer(-operand_int->val);
    }
    if (expr.op == AST::Operator::NEG) {
        if (typeid(*operand) != typeid(Bool)) {
            throw IllegalCastException(this->operator_to_string(expr.op), operand);
        }
        Bool* operand_bool = dynamic_cast<Bool*>(operand);
        DEBUG_PRINT("Exiting unaryOperation" );
        return new Bool(!operand_bool->val);
    }
    return nullptr;
}

Bool* Interpreter::comparisonOperation(const AST::BinaryExpression& expr) {
    DEBUG_PRINT("Entering comparisonOperation with " );
    DEBUG_PRINT_AST(expr, printer);
    
    expr.left->accept(*this);
    Value* left = this->rval;
    expr.right->accept(*this);
    Value* right = this->rval;

    if (typeid(*left) != typeid(Integer)) {
        // Illegal Cast Exception
        throw IllegalCastException(this->operator_to_string(expr.op), left);
    }
    if (typeid(*right) != typeid(Integer)) {
        // Illegal Cast Exception
        throw IllegalCastException(this->operator_to_string(expr.op), right);
    }

    Integer* left_int = dynamic_cast<Integer*>(left);
    Integer* right_int = dynamic_cast<Integer*>(right);

    if (expr.op == AST::Operator::GT) {
        DEBUG_PRINT("Exiting comparisonOperation" );
        return new Bool(left_int->val > right_int->val);
    }
    if (expr.op == AST::Operator::LT) {
        DEBUG_PRINT("Exiting comparisonOperation" );
        return new Bool(left_int->val < right_int->val);
    }
    if (expr.op == AST::Operator::GEQ) {
        DEBUG_PRINT("Exiting comparisonOperation" );
        return new Bool(left_int->val >= right_int->val);
    }
    if (expr.op == AST::Operator::LEQ) {
        DEBUG_PRINT("Exiting comparisonOperation" );
        return new Bool(left_int->val <= right_int->val);
    }
    return nullptr;
}

Bool* Interpreter::equalityOperation(const AST::BinaryExpression& expr) {
    DEBUG_PRINT("Entering equalityOperation with " );
    DEBUG_PRINT_AST(expr, printer);

    expr.left->accept(*this);
    Value* left = this->rval;
    expr.right->accept(*this);
    Value* right = this->rval;

    //PrimitiveEqualityMismatched
    if (typeid(*left) != typeid(*right)) {
        return new Bool(false);
    }

    //PrimitiveEquality
    if (typeid(*left) == typeid(Integer)) {
        Integer* left_int = dynamic_cast<Integer*>(left);
        Integer* right_int = dynamic_cast<Integer*>(right);
        DEBUG_PRINT("Exiting equalityOperation" );
        return new Bool(left_int->val == right_int->val);
    }
    if (typeid(*left) == typeid(Bool)) {
        Bool* left_bool = dynamic_cast<Bool*>(left);
        Bool* right_bool = dynamic_cast<Bool*>(right);
        DEBUG_PRINT("Exiting equalityOperation" );
        return new Bool(left_bool->val == right_bool->val);
    }
    if (typeid(*left) == typeid(String)) {
        String* left_str = dynamic_cast<String*>(left);
        String* right_str = dynamic_cast<String*>(right);
        DEBUG_PRINT("Exiting equalityOperation" );
        return new Bool(left_str->val == right_str->val);
    }

    //NoneEquality
    if (typeid(*left) == typeid(None)) {
        return new Bool(true);
    }

    //FunctionEqualityTrue
    if (typeid(*left) == typeid(Function)) {
        Function* left_fun = dynamic_cast<Function*>(left);
        Function* right_fun = dynamic_cast<Function*>(right);
        bool equal = (left_fun->function_frame == right_fun->function_frame);
        equal = equal && (left_fun->formal_arguments == right_fun->formal_arguments);
        equal = equal && left_fun->body->equals(*right_fun->body); // may not be correct way to check statements 
        return new Bool(equal);
    }

    //RecordEquality
    if (typeid(*left) == typeid(Record)) {
        return new Bool(left == right);
    }

    assert(0);
}

String* Interpreter::stringConcatenation(Value* left, Value* right){
    DEBUG_PRINT("Entering stringConcatenation with " );

    std::string left_str = this->string_cast(left);
    std::string right_str = this->string_cast(right);

    DEBUG_PRINT("Exiting stringConcatenation" );
    return new String(left_str + right_str);

}

