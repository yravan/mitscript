#pragma once
#include "debug.h"
#include "interpreter.h"

void Interpreter::varAssignment(const AST::Assignment& expr){
    DEBUG_PRINT("Entering varAssignment");

    // Resolve the frame
    std::string variable = expr.lhs->name;
    DEBUG_PRINT("LHS variable: " + variable);
    Frame* frame_to_write = lookup_write(stack.top(), variable);

    // Resolve the expression
    expr.expr->accept(*this);
    Value* rhs = rval;
    DEBUG_PRINT("RHS evaluated: " + rhs->to_string());

    // Check that this is a name
    if (typeid(*expr.lhs) != typeid(AST::Expression)){
        throw std::runtime_error("Called varAssignment on non-var-assignment expr " + std::string(typeid(expr.lhs).name()));
    }

    frame_to_write->map[variable] = rhs;
    DEBUG_PRINT("Variable " + variable + " assigned in the frame with value " + rhs->to_string());
    DEBUG_PRINT("Exiting varAssignment");
}

void Interpreter::heapAssignment(const AST::Assignment& expr){
    DEBUG_PRINT("Entering heapAssignment");
    // HeapAssignment
    if (typeid(*expr.lhs) != typeid(AST::FieldDereference)){
        throw std::runtime_error("Called heapAssignment on non-heapAssignment expr " + std::string(typeid(expr.lhs).name()));
    }

    AST::FieldDereference* lhs = dynamic_cast<AST::FieldDereference*>(expr.lhs);
    lhs->baseExpr->accept(*this);
    Value* baseExpr = rval;
    DEBUG_PRINT("Base expression evaluated: " + baseExpr->to_string());

    if (typeid(*baseExpr) != typeid(Record)){
        throw IllegalCastException(".", baseExpr);
    }

    Record* record = dynamic_cast<Record*>(baseExpr);
    std::string field = lhs->field;
    DEBUG_PRINT("Field: " + field);

    expr.expr->accept(*this);
    Value* rhs = rval;
    DEBUG_PRINT("RHS evaluated: " + rhs->to_string());

    record->map[field] = rhs;
    DEBUG_PRINT("Field " + field + " assigned in the record");
    DEBUG_PRINT("Exiting heapAssignment");
}

void Interpreter::heapIndexAssignment(const AST::Assignment& expr){
    DEBUG_PRINT("Entering heapIndexAssignment");
    // HeapIndexAssignment
    if (typeid(*expr.lhs) != typeid(AST::IndexExpression)){
        throw std::runtime_error("Called heapIndexAssignment on non-heapIndexAssignment expr " + std::string(typeid(expr.lhs).name()));
    }

    AST::IndexExpression* lhs = dynamic_cast<AST::IndexExpression*>(expr.lhs);
    lhs->baseExpr->accept(*this);
    Value* baseExpr = rval;
    DEBUG_PRINT("Base expression evaluated: " + baseExpr->to_string());

    if (typeid(*baseExpr) != typeid(Record)){
        throw IllegalCastException(".", baseExpr);
    }

    Record* record = dynamic_cast<Record*>(baseExpr);
    lhs->index->accept(*this);
    Value* index = rval;
    std::string field = string_cast(index);
    DEBUG_PRINT("Index evaluated: " + field);

    expr.expr->accept(*this);
    Value* rhs = rval;
    DEBUG_PRINT("RHS evaluated: " + rhs->to_string());

    record->map[field] = rhs;
    DEBUG_PRINT("Field " + field + " assigned in the record with RHS");
    DEBUG_PRINT("Exiting heapIndexAssignment");
}

void Interpreter::ifStatement(const AST::IfStatement& expr){
    DEBUG_PRINT("Entering ifStatement");
    expr.condition->accept(*this);
    Value* condition = rval;
    DEBUG_PRINT("Condition evaluated: " + condition->to_string());

    if (typeid(*condition) != typeid(Bool)){
        throw IllegalCastException("if", condition);
    }

    Bool* conditionBool = dynamic_cast<Bool*>(condition);
    if (conditionBool->val){
        DEBUG_PRINT("Condition is true, executing thenPart");
        expr.thenPart->accept(*this);
    } else {
        if (expr.elsePart != nullptr){
            DEBUG_PRINT("Condition is false, executing elsePart");
            expr.elsePart->accept(*this);   
        }else{
            DEBUG_PRINT("Condition is false");
        }
    }

    DEBUG_PRINT("Exiting ifStatement");
}

void Interpreter::whileStatement(const AST::WhileLoop& expr){
    //While
    DEBUG_PRINT("Entering whileStatement");
    expr.condition->accept(*this);
    Value* condition = rval;

    if (typeid(*condition) != typeid(Bool)){
        throw IllegalCastException("while", condition);
    }

    Bool* conditionBool = dynamic_cast<Bool*>(condition);
    if (conditionBool->val){
        DEBUG_PRINT("Condition is true, executing Body");
        sequence(*(expr.body), expr);
    } 

    DEBUG_PRINT("Exiting whileStatement");
}

void Interpreter::sequence(const AST::Statement& s1, const AST::Statement& s2){
    s1.accept(*this);
    if (!return_flag){
        //Sequence
        s2.accept(*this);
    }
    //SequenceReturn
}

void Interpreter::returnStatement(const AST::Return& expr){
    expr.expr->accept(*this);
    Value* return_val  = rval;
    rval = return_val;
    return_flag = true;
}


