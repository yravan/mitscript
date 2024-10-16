#pragma once
#include <iostream>
#include "debug.h"
#include "frame.h"
#include "value.h"
#include "exceptions.h"
#include "PrettyPrinter.h"
#include <stack>
#include <set>
#include <cassert>

class Interpreter: public Visitor{
public:
    std::stack<Frame*> stack;
    Frame* global_stack_frame;
    None* none_value;
    PrettyPrinter printer;
    Value * rval;
    bool return_flag;

    Interpreter(){
        // initialize global frame & stack & heap
        global_stack_frame = new Frame();
        none_value = new None();
        stack.push(global_stack_frame);
        create_native_functions();
        return_flag = false;
    }


//--------------------------------------------------------------------------------------------------------------------------------------------------------------
    // Lookup Functions
    Frame * lookup_write(Frame* frame, std::string variable){
        std::set<std::string> global_variables = frame->global_variables;
        if (global_variables.count(variable)){
            //GlobalLookupWrite
            return frame->global_frame;
        }else{
            //LocalLookupWrite
            return frame;
        }
    }
    Value * lookup_read(Frame* frame, std::string variable){
        std::set<std::string> global_variables = frame->global_variables;
        if (global_variables.count(variable)){
            //GlobalLookupRead
            if (frame->global_frame->map.count(variable)){
                return frame->global_frame->map[variable];
            }
        }
        //LocalLookupRead
        if (frame->map.count(variable)){
            return frame->map[variable];
        }
        //ScopedLookupRead
        if (frame->parent_frame != nullptr){
            return lookup_read(frame->parent_frame, variable);
        }
        throw UninitializedVariableException(variable);
    }
//--------------------------------------------------------------------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------------------------------------------------------------------
    // Statements
    void varAssignment(const AST::Assignment& expr);
    void heapAssignment(const AST::Assignment& expr);
    void heapIndexAssignment(const AST::Assignment& expr);
    void ifStatement(const AST::IfStatement& expr);
    void whileStatement(const AST::WhileLoop& expr);
    void sequence(const AST::Statement& s1, const AST::Statement& s2);
    void returnStatement(const AST::Return& expr);

//--------------------------------------------------------------------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------------------------------------------------------------------
    // Expressions
    Value* evalPlus(const AST::BinaryExpression& expr);
    Bool* logicalOperation(const AST::BinaryExpression& expr);
    Integer* arithmeticOperation(const AST::BinaryExpression& expr);
    Integer* arithmeticOperation(Value* left, Value* right);
    Value* unaryOperation(const AST::UnaryExpression& expr);
    Bool* comparisonOperation(const AST::BinaryExpression& expr);
    Bool* equalityOperation(const AST::BinaryExpression& expr);
    String* stringConcatenation(Value* left, Value* right);
//--------------------------------------------------------------------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------------------------------------------------------------------
    // Cast
    std::string string_cast(Value*v);
//--------------------------------------------------------------------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------------------------------------------------------------------
    // Native Functions
    void create_native_functions();
    void print(std::string variable);
    void print(Value* value);
    String* input();
    Integer* intcast(std::string variable);
    Integer* intcast(Value* value);
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
    // Data & Function Expressions
    Value* variableRead(const AST::Expression& expr);
    Record* record(const AST::Record& expr);
    Value* fieldRead(const AST::FieldDereference& expr);
    Value* indexRead(const AST::IndexExpression& expr);
    Function* function(const AST::FunctionDeclaration& expr);
    void globals(const AST::Statement& s);
    void assigns(const AST::Statement& s);
    Value* functionCall(const AST::Call& expr);

//--------------------------------------------------------------------------------------------------------------------------------------------------------------


    void visit(const AST::Block& node){
        DEBUG_PRINT("Entering Block" );
        //Sequence + SequenceReturn
        for (AST::Statement* statement: node.statements){
            if (!return_flag){
                statement->accept(*this);
            }
        }
        DEBUG_PRINT("Exiting Block" );
    }
    void visit(const AST::Global& node){}
    void visit(const AST::Expression& node){
        DEBUG_PRINT("Entering Expression" );
        rval = variableRead(node);
        DEBUG_PRINT("Exiting Expression" );
    }
    void visit(const AST::Statement& node){}
    void visit(const AST::Assignment& node){
        DEBUG_PRINT("Entering Assignment" );
        if (typeid(*node.lhs) == typeid(AST::Expression)){
            varAssignment(node);
        }
        if (typeid(*node.lhs) == typeid(AST::FieldDereference)){
            heapAssignment(node);
        }
        if (typeid(*node.lhs) == typeid(AST::IndexExpression)){
            heapIndexAssignment(node);
        }
        DEBUG_PRINT("Exiting Assignment" );
    }
    void visit(const AST::CallStatement& node){
        DEBUG_PRINT("Entering CallStatement" );
        if (node.callExpr != nullptr){
            node.callExpr->accept(*this);
        }
        DEBUG_PRINT("Exiting CallStatement" );
    }
    void visit(const AST::IfStatement& node){
        DEBUG_PRINT("Entering IfStatement" );
        ifStatement(node);
        DEBUG_PRINT("Exiting IfStatement" );
    }
    void visit(const AST::WhileLoop& node){
        DEBUG_PRINT("Entering WhileLoop" );
        whileStatement(node);
        DEBUG_PRINT("Exiting WhileLoop" );
    }
    void visit(const AST::Return& node){
        DEBUG_PRINT("Entering Return" );
        // Return
        if (node.expr != nullptr){
            node.expr->accept(*this);
        }
        return_flag = true;
        DEBUG_PRINT("Exiting Return" );
    }
    void visit(const AST::FunctionDeclaration& node){
        DEBUG_PRINT("Entering FunctionDeclaration" );
        rval = function(node);
        DEBUG_PRINT("Exiting FunctionDeclaration" );
    }
    void visit(const AST::BinaryExpression& node){
        DEBUG_PRINT("Entering BinaryExpression" );
        switch (node.op) {
            case AST::PLUS:
                rval = evalPlus(node);
                break;
            case AST::SUB:
            case AST::MUL:
            case AST::DIV:
                rval = arithmeticOperation(node);
                break;
            case AST::AND:
            case AST::OR:
                rval = logicalOperation(node);
                break;
            case AST::GT:
            case AST::LT:
            case AST::GEQ:
            case AST::LEQ:
                rval = comparisonOperation(node);
                break;
            case AST::EQ:
                rval = equalityOperation(node);
                break;
            default:
                assert(0);
        }
        DEBUG_PRINT("Exiting BinaryExpression" );
    }
    void visit(const AST::UnaryExpression& node){
        DEBUG_PRINT("Entering UnaryExpression" );
        switch (node.op) {
            case AST::SUB:
            case AST::NEG:
                rval = unaryOperation(node);
                break;
            default:
                assert(0);
        }
        DEBUG_PRINT("Exiting UnaryExpression" );
    }   
    void visit(const AST::FieldDereference& node){
        DEBUG_PRINT("Entering FieldDereference" );
        rval = fieldRead(node);
        DEBUG_PRINT("Exiting FieldDereference" );
    }
    void visit(const AST::IndexExpression& node){
        DEBUG_PRINT("Entering IndexExpression" );
        rval = indexRead(node);
        DEBUG_PRINT("Exiting IndexExpression" );
    }
    void visit(const AST::Call& node){
        DEBUG_PRINT("Entering Call" );
        rval = functionCall(node);
        DEBUG_PRINT("Exiting Call" );
    }
    void visit(const AST::Record& node){
        //Record
        DEBUG_PRINT("Entering Record" );
        rval = record(node);
        DEBUG_PRINT("Exiting Record" );
    }
    void visit(const AST::IntegerConstant& node){
        DEBUG_PRINT("Entering IntegerConstant" );
        rval = new Integer(node.value);
        DEBUG_PRINT("Exiting IntegerConstant" );
    }
    void visit(const AST::StringConstant& node){
        DEBUG_PRINT("Entering StringConstant" );
        rval = new String(node.value);
        DEBUG_PRINT("Exiting StringConstant" );
    }
    void visit(const AST::NoneConstant& node){
        DEBUG_PRINT("Entering NoneConstant" );
        rval = none_value;
        DEBUG_PRINT("Exiting NoneConstant" );
    }
    void visit(const AST::BooleanConstant& node){
        DEBUG_PRINT("Entering BooleanConstant" );
        rval = new Bool(node.value);
        DEBUG_PRINT("Entering BooleanConstant" );
    }
    void visit(const AST::Program& node){
        DEBUG_PRINT("Entering Program" );
        if(node.mainBlock != nullptr){
            node.mainBlock->accept(*this);
        }
        DEBUG_PRINT("Global Frame:" + global_stack_frame->to_string());
        DEBUG_PRINT("Entering Program" );
    }

};

